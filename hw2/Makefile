CXX = g++
CXXFLAGS = -std=c++20 -Wall -Werror

SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
BUILD_DIR = build

SRC = $(SRC_DIR)/parser.cpp
TEST_SRC = $(TEST_DIR)/parser_test.cpp
OBJ = $(BUILD_DIR)/parser.o
TEST_OBJ = $(BUILD_DIR)/parser_test.o

all: $(BUILD_DIR) $(BUILD_DIR)/parser_test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJ)

$(TEST_OBJ): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -c $(TEST_SRC) -o $(TEST_OBJ)

$(BUILD_DIR)/parser_test: $(OBJ) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(TEST_OBJ) -lgtest -lgtest_main -o $(BUILD_DIR)/parser_test

test: $(BUILD_DIR)/parser_test
	$(BUILD_DIR)/parser_test

clean:
	rm -rf $(BUILD_DIR)