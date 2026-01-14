CXX = g++
CXXFLAGS = -Wall -I$(BUILD_DIR) -std=c++17

SRC_DIR = src
BUILD_DIR = build
TARGET = compiler

all: $(TARGET)

$(TARGET): $(BUILD_DIR)/parser.tab.cpp $(BUILD_DIR)/lex.yy.c
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

$(BUILD_DIR)/parser.tab.cpp $(BUILD_DIR)/parser.tab.hpp: $(SRC_DIR)/parser.y | $(BUILD_DIR)
	bison -d $< -o $(BUILD_DIR)/parser.tab.cpp

$(BUILD_DIR)/lex.yy.c: $(SRC_DIR)/lexer.l $(BUILD_DIR)/parser.tab.hpp | $(BUILD_DIR)
	flex -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
