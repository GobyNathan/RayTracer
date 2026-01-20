##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## compile les fichiers et cree un executable
##

NAME := raytracer

SRC_DIR := src

SRC := $(shell find $(SRC_DIR) -name "*.cpp")

OBJ := $(SRC:.cpp=.o)

CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17
CPPFLAGS := -I$(SRC_DIR) -I$(SRC_DIR)/core -I$(SRC_DIR)/material -lsfml-graphics -lsfml-window -lsfml-system

$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) -lconfig++ $(CPPFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

all: $(NAME)

debug: CXXFLAGS += -g3
debug: all

clean:
	rm -f $(OBJ)
	rm -f \#*\#

fclean: clean
	rm -f $(NAME)
	rm -f unit_tests*
	rm -f *.gc*

re: fclean all

TEST_SRC := $(filter-out src/main.cpp,$(SRC))

unit_tests: CXXFLAGS += -I.
unit_tests:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o unit_tests $(TEST_SRC) tests/unit/*.cpp -lcriterion -lconfig++ --coverage

tests_run: unit_tests
	./unit_tests
	gcovr --exclude tests/

.PHONY: all clean fclean re debug unit_tests tests_run
