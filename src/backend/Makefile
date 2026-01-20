NAME		=	seed_core
CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror

BUILD_DIR	=	build

SRC			=	main.c \
\
				

OBJ			=	$(addprefix $(BUILD_DIR), $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(BUILD_DIR) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@echo "Build dir"

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $@
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$@ builded"

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaned."

fclean:
	@rm -rf $(BUILD_DIR) $(NAME)
	@echo "Fcleaned"

re: fclean all

.PHONY: all clean fclean re