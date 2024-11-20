CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic

# directories
SRC_DIR=src
BUILD_DIR=build

# executable name
TARGET=numbersRoyale

# source files
SRC=$(wildcard $(SRC_DIR)/*.cpp)
OBJ=$(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# default target
.DEFAULT_GOAL := all

# make debug
debug: CXXFLAGS += -DDEBUG
debug: clean all

# make all
all: $(BUILD_DIR) $(TARGET)

# create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# make clean	
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

.PHONY: all clean debug
