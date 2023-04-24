# Makefile for minishell

NAME      = minishell
LIBFT     = libft/libft.a
SRC_DIR   = src/
OBJ_DIR   = obj/
CC        = gcc
CFLAGS    = # -Werror -Wall -Wextra
INCFLAGS  = -I include -I $(LIBFT)
LDFLAGS   = 

LDFLAGS		= -L/Users/$(USER)/.brew/opt/readline/lib
CPPFLAGS	= -I/Users/$(USER)/.brew/opt/readline/include

PRSR_DIR  = parser/
PRSR_SRCS = tokenizer env_sub cmd_res helper redirect_checking environment parse_utils main

XCTR_DIR  = executor/
XCTR_SRCS = init_structs executor shell_builtins shell_builtins1 exec_utils redir_io exec_builtin child_process

SRC_FILES += $(addprefix $(PRSR_DIR),$(PRSR_SRCS))
SRC_FILES += $(addprefix $(XCTR_DIR),$(XCTR_SRCS))

SRC       = $(addprefix $(SRC_DIR),$(addsuffix .c,$(SRC_FILES)))
OBJ       = $(addprefix $(OBJ_DIR),$(addsuffix .o,$(SRC_FILES)))

OBJF      = test

GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

ifdef DEBUG
	CFLAGS += -g  -fsanitize=address 
endif

all: libft $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) $(CPPFLAGS) $(INCFLAGS) $(LIBFT)  -o $(NAME) -lreadline
	@echo "$(GREEN)Minishell Compiled.$(RESET)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJF) 
	@$(CC) $(CFLAGS) $(INCFLAGS) $(RL_INC) -c $< -o $@ 

$(OBJF):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)$(PRSR_DIR)
	@mkdir -p $(OBJ_DIR)$(XCTR_DIR)

$(LIBFT):
	@echo "$(GREEN)Building libft ...$(RESET)"
	@$(MAKE) -C libft WITH_BONUS=1

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OBJF)
	@$(MAKE) -C libft clean

fclean: clean
	@rm -rf $(NAME)
	@find . -name ".DS_Store" -delete
	@echo "$(RED)Cleaning ...$(RESET)"

re: fclean all

.PHONY: all clean fclean re
