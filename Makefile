CXX = g++

SRC_FOLDER = src
DEPS_FOLDER = dependencies
BUILD_FOLDER = build/debug

EXECUTABLE_NAME = slang_reader

all: compile run

compile: $(SRC_FOLDER)/deserialiser.cpp
	$(CXX) -I$(DEPS_FOLDER) $(SRC_FOLDER)/deserialiser.cpp -o $(BUILD_FOLDER)/$(EXECUTABLE_NAME)

run: $(BUILD_FOLDER)/$(EXECUTABLE_NAME)
	./$(BUILD_FOLDER)/$(EXECUTABLE_NAME) "in.json"

