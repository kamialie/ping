NAME = ft_ping

SRC = main.c \
		address.c \
		exit.c \
		lib.c \
		options.c \
		output.c \
		packet.c \
		pattern_option.c \
		prepare.c \
		socket.c \
		stats.c \
		utils.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

SRC_DIR = src

OBJ_DIR = obj

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	gcc -o $(NAME) $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c include/ping.h include/lib.h | $(OBJ_DIR)
	gcc $(FLAGS) -c -I include -o $@ $<

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "Object directory is created! (./obj)"

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
