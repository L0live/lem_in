NAME=lem-in
NAME_BONUS=visu-hex

CC=gcc
CFLAGS=-Wall -Wextra -Werror -g
CFLAGS_BONUS=$(CFLAGS) -I/usr/include/freetype2

LDLIBS_BONUS=-lglfw -lGL -lm -ldl -lfreetype

SRCS_FOLDER=srcs/
SRCS_BONUS_FOLDER=srcs_bonus/

SRCS=main.c utils.c parsing.c structs_utils.c debug.c free.c ants.c breadthfirst_search.c path_utils.c
SRCS_BONUS=main_bonus.c rooms.c pipe.c font.c shaders.c ants.c gl.c ../glad/src/glad.c
SRCS_BONUS_COMMON = utils.c parsing.c structs_utils.c debug.c free.c path_utils.c

OBJS_FOLDER=objs/
OBJS_BONUS_FOLDER = objs_bonus/

OBJS=$(addprefix $(OBJS_FOLDER), $(SRCS:.c=.o))
OBJS_BONUS=$(addprefix $(OBJS_BONUS_FOLDER), $(SRCS_BONUS:.c=.o))
OBJS_BONUS_COMMON=$(addprefix $(OBJS_BONUS_FOLDER), $(SRCS_BONUS_COMMON:.c=.o))

OBJS_BONUS_ALL = $(OBJS_BONUS) $(OBJS_BONUS_COMMON)

HEADERS=includes/lem_in.h libft/libft.h
HEADERS_BONUS=includes/lem_in_bonus.h glad/include/glad/glad.h glad/include/KHR/khrplatform.h includes/linmath.h
LIBFT=libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $@
	@echo "\n\nlem_in builded"

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(LIBFT) $(OBJS_BONUS_ALL)
	@$(CC) $(CFLAGS_BONUS) $(OBJS_BONUS_ALL) $(LIBFT) $(LDLIBS_BONUS) -o $@
	@echo "\n\nlem_in bonus builded"

# Règle de compilation des .o dans objs/ pour la version principale
$(OBJS_FOLDER)%.o: $(SRCS_FOLDER)%.c $(HEADERS)
	@mkdir -p $(OBJS_FOLDER)
	@$(CC) $(CFLAGS) -c $< -o $@ -I .
	@echo "Compiling $<..."

# Règle de compilation des .o dans objs/ pour la version bonus
$(OBJS_BONUS_FOLDER)%.o: $(SRCS_BONUS_FOLDER)%.c $(HEADERS) $(HEADERS_BONUS)
	@mkdir -p $(OBJS_BONUS_FOLDER)
	@$(CC) $(CFLAGS_BONUS) -c $< -o $@ -I .
	@echo "Compiling $<..."

$(OBJS_BONUS_FOLDER)%.o: $(SRCS_FOLDER)%.c $(HEADERS) $(HEADERS_BONUS)
	@mkdir -p $(OBJS_BONUS_FOLDER)
	@$(CC) $(CFLAGS_BONUS) -I. -c $< -o $@
	@echo "Compiling $<..."

clean:
	@rm -rf $(OBJS_FOLDER) $(OBJS_BONUS_FOLDER)
	@$(MAKE) -s -C libft/ clean
	@echo "All objects cleaned"

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS)
	@$(MAKE) -s -C libft/ fclean
	@echo "Executable removed"

$(LIBFT):
	@$(MAKE) -s -C libft/ all

re: fclean all

.PHONY: all bonus fclean clean re