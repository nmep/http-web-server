# FILE
NAME = webserv

SRC = $(shell find -type f -name "*.cpp" | cut -c 3-)
OBJ = $(addprefix obj/, $(SRC:.cpp=.o))
OBJ_DIR = $(addprefix ./obj/, $(notdir $(shell find ./ -type d -not -path ./conf -not -path ./include -not -path ./obj)))

# COMPILATION
CC = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I./include -g3

# RULES
./obj/%.o: %.cpp | create_dir
	$(CC) $(CXXFLAGS) $< -c -o $@

$(NAME): $(OBJ)
	$(CC) $(CXXFLAGS) $(OBJ) -o $@

all: $(NAME)

clean:
	rm -rf $(OBJ) $(OBJ_DIR) ./obj

fclean: clean
	rm -f $(NAME)

re: fclean all

create_dir:
	$(shell mkdir -p $(OBJ_DIR))

test:
	@echo src $(SRC)
	@echo obj $(OBJ)
	@echo objDir $(OBJ_DIR)

.PHONY: all clean fclean re
