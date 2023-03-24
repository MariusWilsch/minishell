/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 10:38:21 by verdant           #+#    #+#             */
/*   Updated: 2023/03/24 12:15:58 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "../libft/include/libft.h"
# include "../libft/include/ft_printf.h"


/**
 * @brief Gives high level meaning to the tokens
 * 
 * @param OPERATOR a token which includes a redirect or a pipe
 * @param ARG a token which can be a command (if it's the first) or a argument
 * @param QUOTE_ARG Chained arguments. Special chars other than $ lose their meaning
*/
typedef enum s_tokens {
	OPERATOR,
	ARG,
	QUOTE_ARG,
	CMD,
	BUILT_IN,
	REDIRECT,
	REPROMPT,
} t_type_tok;

typedef enum s_err_toks {
	OK,
	NO_CMD,
	NO_FILE_DIR,
	TO_MANY,
	NO_ALNUM_BETWEEN,
	NEWLINE_ERR,
	ENV_REDIRECT_ERR,
	// MALLOC_ERR,
} t_err_tok;

typedef struct t_args {
	char					*arg;
	t_type_tok		type;
	t_err_tok			err_tok;
	struct t_args	*next;
} t_args;

// typedef struct s_cmd {
// 	char *cmd; // The resoluted cmd
// 	// int		num_args;		//	The number of arguments for the command
// 	char	**args;			//	The names of the arguments for the command
// 	char	**oper;			//	Operaters & their files
// 	// void (*func)();		//	The function pointer to the implementation code
// } t_cmds;



/*			Helper_1			*/

bool		incl_char(char c, char *search_str);
bool		are_quotes_even(char *input);
char		*del_substr(char *str, int start, int len);
int			cnt_len_between(char *str, char c, int index);
char		*del_quotes(char *str);
int			cnt_occur(char *str, char c);

/*			Tokenizer			*/

char		*get_tok(char *input, int start, t_type_tok type);
t_args	*create_node(char *str, t_type_tok type);
int			add_tok(char *str, t_args **head, t_type_tok type);
t_args	*create_tok_list(char *input, t_args *head);
t_args	*process_tok(t_args *head);


/*			Command Resolution			*/

bool	is_builtin(t_args *node);
char	*cmd_err(t_args *node);
char	*prep_cmd(char *str);
char	*resolute_cmd(t_args *node, char *cmd);

/*			Environment Subsitution			*/

int		get_env_len(char *str);
char	*sub_env(char *str, int env_len);

/*			Redirect Checking			*/

int	err_msg(t_err_tok err);
int	env_var_case(char *str, int env_len, int cnt);
int	check_redirect(char *str, char c, int cnt, t_args *node);


/*			Executor			*/

bool executor(t_args *head);

#endif