NAME=lem_in
NAME_BONUS=lem_in_bonus
CC=gcc
CFLAGS=-Wall -Wextra -Werror -g
SRCS_FOLDER=srcs/
# SRCS_BONUS_FOLDER=srcs_bonus/
SRCS=main.c 
# SRCS_BONUS=main_bonus.c 
OBJS_FOLDER=objs/
OBJS=$(addprefix $(OBJS_FOLDER), $(SRCS:.c=.o))
OBJS_BONUS=$(addprefix $(OBJS_FOLDER), $(SRCS_BONUS:.c=.o))
HEADERS=srcs/lem_in.h libft/libft.h
# HEADERS_BONUS=srcs_bonus/lem_in_bonus.h libft/libft.h
# MLX=minilibx-linux/libmlx.a
LIBFT=libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $@
	@echo "\n\nlem_in builded"

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(LIBFT) $(OBJS_BONUS)
	@$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT) -o $@
	@echo "\n\nlem_in bonus builded"

# Règle de compilation des .o dans objs/ pour la version principale
$(OBJS_FOLDER)%.o: $(SRCS_FOLDER)%.c $(HEADERS)
	@mkdir -p $(OBJS_FOLDER)
	@$(CC) $(CFLAGS) -c $< -o $@ -I .
	@echo "Compiling $<..."

# Règle de compilation des .o dans objs/ pour la version bonus
$(OBJS_FOLDER)%.o: $(SRCS_BONUS_FOLDER)%.c $(HEADERS_BONUS)
	@mkdir -p $(OBJS_FOLDER)
	@$(CC) $(CFLAGS) -c $< -o $@ -I .
	@echo "Compiling $<..."

clean:
	@rm -f $(OBJS) $(OBJS_BONUS)
	@$(MAKE) -s -C libft/ clean
	@echo "All objects cleaned"

fclean: clean
	@rm -f $(NAME) $(NAME)_bonus
	@$(MAKE) -s -C libft/ fclean
	@echo "Executable removed"

$(LIBFT):
	@$(MAKE) -s -C libft/ all

re: fclean all

.PHONY: all bonus fclean clean re