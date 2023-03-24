# Makefile for minishell

NAME      = minishell
LIBFT     = libft
SRC_DIR   = src/
OBJ_DIR   = obj/
CC        = gcc
CFLAGS    = # -Werror -Wall -Wextra
INCFLAGS  = -I include
LDFLAGS   = -lreadline
RM        = rm -rf


PRSR_DIR  = parser/
PRSR_SRCS = tokenizer env_sub cmd_res helper redirect_checking main

XCTR_DIR  = executor/
XCTR_SRCS = init_structs executor

SRC_FILES += $(addprefix $(PRSR_DIR),$(PRSR_SRCS))
SRC_FILES += $(addprefix $(XCTR_DIR),$(XCTR_SRCS))

SRC       = $(addprefix $(SRC_DIR),$(addsuffix .c,$(SRC_FILES)))
OBJ       = $(addprefix $(OBJ_DIR),$(addsuffix .o,$(SRC_FILES)))

OBJF      = test

# Colors
C_RESET = "\033[0m"
C_GREEN = "\033[92m"

ifdef DEBUG
	CFLAGS += -g -fsanitize=address
endif

start:
	@make -C $(LIBFT)
	@cp $(LIBFT)/libft.a .
	@make all

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) libft.a -o $(NAME)
	@echo $(C_GREEN)"minishell compiled"$(C_RESET)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJF)
	@$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(OBJF):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)$(PRSR_DIR)
	@mkdir -p $(OBJ_DIR)$(XCTR_DIR)

clean:
	@$(RM) $(OBJ_DIR)
	@$(RM) $(OBJF)
	@make clean -C $(LIBFT)

fclean: clean
	@$(RM) $(NAME)
	@$(RM) $(LIBFT)/libft.a
	@$(RM) libft.a
	@find . -name ".DS_Store" -delete
	@echo "$(Red)All libs cleaned$(Reset)"

re: fclean all
	@echo "$(Yellow)Recompiled everything$(Reset)"

.PHONY: all clean fclean re
