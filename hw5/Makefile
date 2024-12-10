CXX = g++
CXXFLAGS = -std=c++20 -Wall -Werror

INCLUDE_DIR = include
TEST_DIR = tests
BUILD_DIR = build

TEST_SRC = $(TEST_DIR)/serializer_test.cpp
TEST_OBJ = $(BUILD_DIR)/serializer_test.o

all: $(BUILD_DIR) $(BUILD_DIR)/serializer_test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TEST_OBJ): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -c $(TEST_SRC) -o $(TEST_OBJ)

$(BUILD_DIR)/serializer_test: $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(TEST_OBJ) -lgtest -lgtest_main -o $(BUILD_DIR)/serializer_test

test: $(BUILD_DIR)/serializer_test
	$(BUILD_DIR)/serializer_test

clean:
	rm -rf $(BUILD_DIR)