CXX = g++
CXXFLAGS = -g -Wall -I$(SRC_DIR) -I$(BUILD_DIR) -std=c++17

SRC_DIR = src
BUILD_DIR = build
TARGET = kompilator

SRCS = $(SRC_DIR)/codegen.cpp $(SRC_DIR)/symtable.cpp $(SRC_DIR)/math_kernel.cpp $(SRC_DIR)/ast.cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(BUILD_DIR)/parser.tab.cpp $(BUILD_DIR)/lex.yy.c $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lcln

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

.PHONY: all clean
