##
## EPITECH PROJECT, 2025
## B-OOP-400-TLS-4-1-bsraytracer-perrine.feyt
## File description:
## Makefile
##

SRC =	main.cpp \
		$(wildcard src/*.cpp) \

OBJ = $(SRC:.cpp=.o)

NAME = raytracer

all: $(NAME)

$(NAME): $(OBJ)
	g++ -o $(NAME) $(OBJ) -std=c++20 -Wall -Wextra -Werror

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
