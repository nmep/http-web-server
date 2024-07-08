# FILE
NAME = webserv

SRC = $(shell find ./ -type f -name "*.cpp")
INC = ./include/
OBJ = $(SRC:.cpp=.o)

# COMPILATION

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -I./include

# RULE

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@


$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $@

clean:
	rm -f $(OBJ)

fclean:
	rm -f $(OBJ) $(NAME)

test:
	@echo $(SRC)
	@echo $(OBJ)
	@echo $(INC)