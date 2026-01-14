CXX = g++
CXXFLAGS = -Wall -I$(SRC_DIR) -I$(BUILD_DIR) -std=c++17

SRC_DIR = src
BUILD_DIR = build
TARGET = compiler

# List of source files (excluding generated ones)
SRCS = $(SRC_DIR)/codegen.cpp $(SRC_DIR)/symtable.cpp $(SRC_DIR)/math_kernel.cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(BUILD_DIR)/parser.tab.cpp $(BUILD_DIR)/lex.yy.c $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

# Compile C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/parser.tab.cpp $(BUILD_DIR)/parser.tab.hpp: $(SRC_DIR)/parser.y | $(BUILD_DIR)
	bison -d $< -o $(BUILD_DIR)/parser.tab.cpp

$(BUILD_DIR)/lex.yy.c: $(SRC_DIR)/lexer.l $(BUILD_DIR)/parser.tab.hpp | $(BUILD_DIR)
	flex -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Test targets
# Usage: make test CATEGORY=example [FAIL_FAST=1]
# CATEGORY - test category name (required, e.g., example, labor4, custom, standard)
# FAIL_FAST - if set to 1, stops on first failed test (optional, default: 0)

VM ?= ./mw2025/maszyna-wirtualna
TESTS_DIR = tests
CASES_DIR = $(TESTS_DIR)/cases
IN_DIR = $(TESTS_DIR)/in
OUT_DIR = $(TESTS_DIR)/out
ANS_DIR = $(TESTS_DIR)/ans
MR_DIR = $(TESTS_DIR)/mr
FAIL_FAST ?= 0

test:
	@if [ -z "$(CATEGORY)" ]; then \
		echo "Error: CATEGORY not specified"; \
		echo "Usage: make test CATEGORY=<category> [FAIL_FAST=1]"; \
		echo "Available categories:"; \
		ls -1 $(CASES_DIR); \
		exit 1; \
	fi
	@if [ ! -d "$(CASES_DIR)/$(CATEGORY)" ]; then \
		echo "Error: Category '$(CATEGORY)' not found in $(CASES_DIR)"; \
		echo "Available categories:"; \
		ls -1 $(CASES_DIR); \
		exit 1; \
	fi
	@echo "Running tests for category: $(CATEGORY)"
	@echo "Fail fast: $(FAIL_FAST)"
	@echo "=========================================="
	@mkdir -p $(MR_DIR)/$(CATEGORY) $(OUT_DIR)
	@passed=0; failed=0; \
	for test_file in $(CASES_DIR)/$(CATEGORY)/*.imp; do \
		test_name=$$(basename $$test_file .imp); \
		mr_file=$(MR_DIR)/$(CATEGORY)/$$test_name.mr; \
		in_file=$(IN_DIR)/$(CATEGORY)/$$test_name.in; \
		out_file=$(OUT_DIR)/$(CATEGORY)/$$test_name.out; \
		ans_file=$(ANS_DIR)/$(CATEGORY)/$$test_name.ans; \
		echo -n "Testing $$test_name... "; \
		if [ ! -f "$$test_file" ]; then \
			echo "SKIP (source not found)"; \
			continue; \
		fi; \
		./$(TARGET) $$test_file $$mr_file > /dev/null 2>&1; \
		if [ $$? -ne 0 ]; then \
			echo "FAIL (compilation error)"; \
			failed=$$((failed + 1)); \
			if [ "$(FAIL_FAST)" = "1" ]; then \
				echo "Stopping due to FAIL_FAST=1"; \
				exit 1; \
			fi; \
			continue; \
		fi; \
		if [ ! -f "$$mr_file" ]; then \
			echo "FAIL (no .mr file generated)"; \
			failed=$$((failed + 1)); \
			if [ "$(FAIL_FAST)" = "1" ]; then \
				echo "Stopping due to FAIL_FAST=1"; \
				exit 1; \
			fi; \
			continue; \
		fi; \
		if [ -f "$$in_file" ]; then \
			$(VM) $$mr_file < $$in_file > $$out_file.tmp 2>&1; \
		else \
			$(VM) $$mr_file > $$out_file.tmp 2>&1; \
		fi; \
		vm_status=$$?; \
		cat $$out_file.tmp | sed -r "s/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[mGK]//g" | grep -vE "^Czytanie kodu|^Skończono czytanie|^Uruchamianie programu|^Skończono program \(koszt:" > $$out_file; \
		rm $$out_file.tmp; \
		if [ $$vm_status -ne 0 ]; then \
			echo "FAIL (runtime error)"; \
			failed=$$((failed + 1)); \
			if [ "$(FAIL_FAST)" = "1" ]; then \
				echo "Stopping due to FAIL_FAST=1"; \
				exit 1; \
			fi; \
			continue; \
		fi; \
		if [ ! -f "$$ans_file" ]; then \
			echo "SKIP (no answer file)"; \
			continue; \
		fi; \
		if diff -q $$out_file $$ans_file > /dev/null 2>&1; then \
			echo "PASS"; \
			passed=$$((passed + 1)); \
		else \
			echo "FAIL (output mismatch)"; \
			echo "  Expected: $$ans_file"; \
			echo "  Got:      $$out_file"; \
			echo "  Diff:"; \
			diff $$ans_file $$out_file | head -20 | sed 's/^/    /'; \
			failed=$$((failed + 1)); \
			if [ "$(FAIL_FAST)" = "1" ]; then \
				echo "Stopping due to FAIL_FAST=1"; \
				exit 1; \
			fi; \
		fi; \
	done; \
	echo "=========================================="; \
	echo "Results: $$passed passed, $$failed failed"; \
	if [ $$failed -gt 0 ]; then \
		exit 1; \
	fi

# Run a single program
# Usage: make run FILE=<path/to/program.imp>
run: $(TARGET)
	@if [ -z "$(FILE)" ]; then \
		echo "Error: FILE not specified"; \
		echo "Usage: make run FILE=<path/to/program.imp>"; \
		exit 1; \
	fi
	@if [ ! -f "$(FILE)" ]; then \
		echo "Error: File '$(FILE)' not found"; \
		exit 1; \
	fi
	@echo "Compiling $(FILE)..."
	@./$(TARGET) $(FILE) output.mr
	@echo "Running output.mr..."
	@$(VM) output.mr

.PHONY: all clean test run
