#melissaliebowitz@yahoo.com
CXX = clang++-18
CXXFLAGS = -std=c++17 -Wall -Iinclude


LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system


SRC_D = src
INC_D = include
TEST_D = tests


SRC_FILES = $(wildcard $(SRC_D)/*.cpp)
SRC_NO_MAIN = $(filter-out $(SRC_D)/main.cpp, $(SRC_FILES))
TEST_FILES = $(wildcard $(TEST_D)/*.cpp)

OBJ_FILES = $(SRC_FILES:.cpp=.o)

MAIN_BIN = main
TEST_BIN = test


$(MAIN_BIN): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	./$(MAIN_BIN)


$(SRC_D)/%.o: $(SRC_D)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


test: $(SRC_NO_MAIN) $(TEST_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $(TEST_BIN)
	./$(TEST_BIN)

valgrind: main
	valgrind --leak-check=full --track-origins=yes ./main

clean:
	rm -f $(SRC_D)/*.o $(MAIN_BIN) $(TEST_BIN)



