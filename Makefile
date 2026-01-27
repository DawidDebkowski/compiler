CXX = g++
CXXFLAGS = -g -Wall -I$(SRC_DIR) -I$(BUILD_DIR) -std=c++17

SRC_DIR = src
BUILD_DIR = build
TARGET = compiler

# List of source files (excluding generated ones)
SRCS = $(SRC_DIR)/codegen.cpp $(SRC_DIR)/symtable.cpp $(SRC_DIR)/math_kernel.cpp $(SRC_DIR)/ast.cpp $(SRC_DIR)/tac.cpp $(SRC_DIR)/tac_backend.cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(BUILD_DIR)/parser.tab.cpp $(BUILD_DIR)/lex.yy.c $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm -lcln

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

VM ?= ./mw2025/maszyna-wirtualna-cln
TESTS_DIR = tests
CASES_DIR = $(TESTS_DIR)/cases
IN_DIR = $(TESTS_DIR)/in
OUT_DIR = $(TESTS_DIR)/out
ANS_DIR = $(TESTS_DIR)/ans
MR_DIR = $(TESTS_DIR)/mr
LOGS_DIR = logs
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
		if [ ! -f "$$test_file" ]; then \
			echo "Testing $$test_name... SKIP (source not found)"; \
			continue; \
		fi; \
		./$(TARGET) $$test_file $$mr_file > /dev/null 2>&1; \
		if [ $$? -ne 0 ]; then \
			echo "Testing $$test_name... FAIL (compilation error)"; \
			failed=$$((failed + 1)); \
			if [ "$(FAIL_FAST)" = "1" ]; then \
				echo "Stopping due to FAIL_FAST=1"; \
				exit 1; \
			fi; \
			continue; \
		fi; \
		if [ ! -f "$$mr_file" ]; then \
			echo "Testing $$test_name... FAIL (no .mr file generated)"; \
			failed=$$((failed + 1)); \
			if [ "$(FAIL_FAST)" = "1" ]; then \
				echo "Stopping due to FAIL_FAST=1"; \
				exit 1; \
			fi; \
			continue; \
		fi; \
		inputs_list=""; \
		has_inputs=0; \
		if [ -f "$(IN_DIR)/$(CATEGORY)/$$test_name.in" ]; then \
			inputs_list="$$inputs_list $(IN_DIR)/$(CATEGORY)/$$test_name.in"; \
			has_inputs=1; \
		fi; \
		multi_inputs=$$(find $(IN_DIR)/$(CATEGORY) -maxdepth 1 -name "$$test_name#*.in" 2>/dev/null | sort); \
		if [ -n "$$multi_inputs" ]; then \
			inputs_list="$$inputs_list $$multi_inputs"; \
			has_inputs=1; \
		fi; \
		if [ $$has_inputs -eq 0 ]; then \
			inputs_list="NO_INPUT"; \
		fi; \
		for in_file_path in $$inputs_list; do \
			if [ "$$in_file_path" = "NO_INPUT" ]; then \
				display_name="$$test_name"; \
				cur_in=""; \
				cur_out="$(OUT_DIR)/$(CATEGORY)/$$test_name.out"; \
				cur_ans="$(ANS_DIR)/$(CATEGORY)/$$test_name.ans"; \
			else \
				base_in=$$(basename $$in_file_path .in); \
				suffix=$${base_in#$$test_name}; \
				if [ -z "$$suffix" ]; then \
					display_name="$$test_name"; \
				else \
					display_name="$$test_name (input$$suffix)"; \
				fi; \
				cur_in="$$in_file_path"; \
				cur_out="$(OUT_DIR)/$(CATEGORY)/$$base_in.out"; \
				cur_ans="$(ANS_DIR)/$(CATEGORY)/$$base_in.ans"; \
			fi; \
			echo -n "Testing $$display_name... "; \
			if [ -n "$$cur_in" ]; then \
				$(VM) $$mr_file < $$cur_in > $$cur_out.tmp 2>&1; \
			else \
				$(VM) $$mr_file > $$cur_out.tmp 2>&1; \
			fi; \
			vm_status=$$?; \
			cat $$cur_out.tmp | sed -r "s/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[mGK]//g" | grep -vE "^Czytanie kodu|^Skończono czytanie|^Uruchamianie programu|^Skończono program \(koszt:" > $$cur_out; \
			cost=$$(cat $$cur_out.tmp | sed -r "s/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[mGK]//g" | grep "Skończono program (koszt:" | sed -E 's/.*koszt:[[:space:]]*([^;]+);.*/\1/'); \
			instr=$$(cat $$cur_out.tmp | sed -r "s/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[mGK]//g" | grep "Skończono czytanie kodu (liczba rozkazów:" | sed -E 's/.*liczba rozkazów: ([0-9 ]+).*/\1/'); \
			rm $$cur_out.tmp; \
			if [ $$vm_status -ne 0 ]; then \
				echo "FAIL (runtime error)"; \
				failed=$$((failed + 1)); \
				if [ "$(FAIL_FAST)" = "1" ]; then \
					echo "Stopping due to FAIL_FAST=1"; \
					exit 1; \
				fi; \
				continue; \
			fi; \
			if [ ! -f "$$cur_ans" ]; then \
				echo "SKIP (no answer file)"; \
				continue; \
			fi; \
			if diff -q $$cur_out $$cur_ans > /dev/null 2>&1; then \
				echo "PASS (Cost: $$cost, Instructions: $$instr)"; \
				passed=$$((passed + 1)); \
			else \
				echo "FAIL (output mismatch)"; \
				echo "  Expected: $$cur_ans"; \
				echo "  Got:      $$cur_out"; \
				echo "  Diff:"; \
				diff $$cur_ans $$cur_out | head -20 | sed 's/^/    /'; \
				failed=$$((failed + 1)); \
				if [ "$(FAIL_FAST)" = "1" ]; then \
					echo "Stopping due to FAIL_FAST=1"; \
					exit 1; \
				fi; \
			fi; \
		done; \
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
	@./$(TARGET) $(FILE) output.mr output.ast
	@echo "Running output.mr..."
	@$(VM) output.mr

test-all: $(TARGET)
	@mkdir -p $(LOGS_DIR)
	@timestamp=$$(date +"%d%H%M%S"); \
	log_file=$(LOGS_DIR)/$${timestamp}_test.log; \
	echo "Running tests for all categories..."; \
	echo "Log file: $$log_file"; \
	echo "==========================================" | tee $$log_file; \
	echo "Test Run Report - $$timestamp" | tee -a $$log_file; \
	echo "==========================================" | tee -a $$log_file; \
	echo "" | tee -a $$log_file; \
	total_passed=0; total_failed=0; \
	for category in $(CASES_DIR)/*; do \
		if [ ! -d "$$category" ]; then continue; fi; \
		cat_name=$$(basename $$category); \
		echo "" | tee -a $$log_file; \
		echo "### Category: $$cat_name ###" | tee -a $$log_file; \
		echo "==========================================" | tee -a $$log_file; \
		mkdir -p $(MR_DIR)/$$cat_name $(OUT_DIR)/$$cat_name; \
		passed=0; failed=0; \
		for test_file in $$category/*.imp; do \
			if [ ! -f "$$test_file" ]; then continue; fi; \
			test_name=$$(basename $$test_file .imp); \
			mr_file=$(MR_DIR)/$$cat_name/$$test_name.mr; \
			./$(TARGET) $$test_file $$mr_file > /dev/null 2>&1; \
			if [ $$? -ne 0 ]; then \
				printf "  %-30s " "$$test_name..." | tee -a $$log_file; \
				echo "FAIL (compilation error)" | tee -a $$log_file; \
				failed=$$((failed + 1)); \
				if [ "$(FAIL_FAST)" = "1" ]; then \
					echo "Stopping due to FAIL_FAST=1" | tee -a $$log_file; \
					exit 1; \
				fi; \
				continue; \
			fi; \
			if [ ! -f "$$mr_file" ]; then \
				printf "  %-30s " "$$test_name..." | tee -a $$log_file; \
				echo "FAIL (no .mr file generated)" | tee -a $$log_file; \
				failed=$$((failed + 1)); \
				if [ "$(FAIL_FAST)" = "1" ]; then \
					echo "Stopping due to FAIL_FAST=1" | tee -a $$log_file; \
					exit 1; \
				fi; \
				continue; \
			fi; \
			inputs_list=""; \
			has_inputs=0; \
			if [ -f "$(IN_DIR)/$$cat_name/$$test_name.in" ]; then \
				inputs_list="$$inputs_list $(IN_DIR)/$$cat_name/$$test_name.in"; \
				has_inputs=1; \
			fi; \
			multi_inputs=$$(find $(IN_DIR)/$$cat_name -maxdepth 1 -name "$$test_name#*.in" 2>/dev/null | sort); \
			if [ -n "$$multi_inputs" ]; then \
				inputs_list="$$inputs_list $$multi_inputs"; \
				has_inputs=1; \
			fi; \
			if [ $$has_inputs -eq 0 ]; then \
				inputs_list="NO_INPUT"; \
			fi; \
			for in_file_path in $$inputs_list; do \
				if [ "$$in_file_path" = "NO_INPUT" ]; then \
					display_name="$$test_name"; \
					cur_in=""; \
					cur_out="$(OUT_DIR)/$$cat_name/$$test_name.out"; \
					cur_ans="$(ANS_DIR)/$$cat_name/$$test_name.ans"; \
				else \
					base_in=$$(basename $$in_file_path .in); \
					suffix=$${base_in#$$test_name}; \
					if [ -z "$$suffix" ]; then \
						display_name="$$test_name"; \
					else \
						display_name="$$test_name (input$$suffix)"; \
					fi; \
					cur_in="$$in_file_path"; \
					cur_out="$(OUT_DIR)/$$cat_name/$$base_in.out"; \
					cur_ans="$(ANS_DIR)/$$cat_name/$$base_in.ans"; \
				fi; \
				printf "  %-30s " "$$display_name..." | tee -a $$log_file; \
				if [ -n "$$cur_in" ]; then \
					$(VM) $$mr_file < $$cur_in > $$cur_out.tmp 2>&1; \
				else \
					$(VM) $$mr_file > $$cur_out.tmp 2>&1; \
				fi; \
				vm_exit=$$?; \
				cat $$cur_out.tmp | sed -r "s/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[mGK]//g" | grep -vE "^Czytanie kodu|^Skończono czytanie|^Uruchamianie programu|^Skończono program \(koszt:" > $$cur_out; \
				cost=$$(cat $$cur_out.tmp | sed -r "s/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[mGK]//g" | grep "Skończono program (koszt:" | sed -E 's/.*koszt:[[:space:]]*([^;]+);.*/\1/'); \
				instr=$$(cat $$cur_out.tmp | sed -r "s/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[mGK]//g" | grep "Skończono czytanie kodu (liczba rozkazów:" | sed -E 's/.*liczba rozkazów: ([0-9 ]+).*/\1/'); \
				rm $$cur_out.tmp; \
				if [ $$vm_exit -ne 0 ]; then \
					echo "FAIL (runtime error, exit code: $$vm_exit)" | tee -a $$log_file; \
					failed=$$((failed + 1)); \
					if [ "$(FAIL_FAST)" = "1" ]; then \
						echo "Stopping due to FAIL_FAST=1" | tee -a $$log_file; \
						exit 1; \
					fi; \
					continue; \
				fi; \
				if [ ! -f "$$cur_ans" ]; then \
					echo "SKIP (no answer file)" | tee -a $$log_file; \
					continue; \
				fi; \
				if diff -q $$cur_out $$cur_ans > /dev/null 2>&1; then \
					echo "PASS (Cost: $$cost, Instructions: $$instr)" | tee -a $$log_file; \
					passed=$$((passed + 1)); \
				else \
					echo "FAIL (output mismatch)" | tee -a $$log_file; \
					echo "    Expected: $$cur_ans" >> $$log_file; \
					echo "    Got:      $$cur_out" >> $$log_file; \
					echo "    Diff:" >> $$log_file; \
					diff $$cur_ans $$cur_out | head -20 | sed 's/^/      /' >> $$log_file; \
					failed=$$((failed + 1)); \
					if [ "$(FAIL_FAST)" = "1" ]; then \
						echo "Stopping due to FAIL_FAST=1" | tee -a $$log_file; \
						exit 1; \
					fi; \
				fi; \
			done; \
		done; \
		echo "  Category results: $$passed passed, $$failed failed" | tee -a $$log_file; \
		total_passed=$$((total_passed + passed)); \
		total_failed=$$((total_failed + failed)); \
	done; \
	echo "" | tee -a $$log_file; \
	echo "==========================================" | tee -a $$log_file; \
	echo "TOTAL RESULTS: $$total_passed passed, $$total_failed failed" | tee -a $$log_file; \
	echo "==========================================" | tee -a $$log_file; \
	echo "Log saved to: $$log_file"; \
	if [ $$total_failed -gt 0 ]; then \
		exit 1; \
	fi

.PHONY: all clean test run test-all
