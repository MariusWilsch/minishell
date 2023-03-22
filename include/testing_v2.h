/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing_v2.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 20:22:26 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/22 13:01:11 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTING_H
#define TESTING_H

# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "../libft/include/libft.h"
# include "../libft/include/ft_printf.h"

typedef enum tokens {
	TOO_MANY,
	NO_ALNUM_1,
	NEWLINE,
	NO_FILE,
	AMBIGOUS_REDIRECT,
	DENIED,
} t_err_token;


typedef struct s_command {
	char	*name;			//	The name of the command
	// int		num_args;		//	The number of arguments for the command
	char	**args;			//	The names of the arguments for the command
	// char	**oper;			//	Operaters & their files
	void (*func)();		//	The function pointer to the implementation code
} t_cmd;


/**
 * @brief Struct holding necessary data
 * 
 * @param fd For file creation. Proabaly have to do a arr of fd's 
 * becauese It my happen that I need to create multiple fd's
*/
typedef struct s_data {
	int		cmd_cnt;
	int		spc_cmd_len;
	int		env_len;
	int		env_start;
	int		env_size;
	int		redir_cnt;
	char	redir_char;
	int		fd;
	char	*delim;
	char	*skip;
} t_data;


/**
 * @brief Utility functions
 * 
 * @param skip or char *skip is used to indicate that characters between quotes
 * are not meant to be evaluated
 */

bool	are_quotes_even(char *input);
bool	incl_char(char c, char *search_str);
int		ft_search_c(char *str, char c, char skip);
int		count_occurences_skip(char *str, char c, char *skip);
void	err_msg(char *msg);


/**
 * @brief Helper functions for environment variable substitution
 * 
 * @param env_len is determined within get_env and excludes the $ sign
 * therefore if the dollar sign needs to be considered it's needs to be
 * incremented by 1
 * 
 * @note copy_before is used in order to copy all chars from string before the env.
 * variable
*/

int		copy_before(char **temp, char *str, t_data *data);
char	*get_env(char *str, t_data *data);
char	*substitute_var(char *str, char *env_var, int env_len, t_data *data);
bool	env_res(char **str, t_data *data);


/**
 * @brief All functions for redirection
*/

char	*cut_out(char *str, int start, t_data *data);
bool	check_sematics(char *str, char symbol, int cnt, t_data *data);
bool	check_syntax(char *str, char c, int cnt, t_data *data);
bool	redirect_pars(char *str, t_data *data);
int		scan_string(char *str);

/**
 * @brief All functions for command resolution
*/

char	*add_to_arr(char *string, char *str_to_add, int size);
bool	prep_cmd(char *str, t_cmd *cmd, t_data *data);
bool	cmd_res(char *str, t_cmd *cmd, t_data *data);



char	*del_substr(char *str, int start, int len);
bool error_syntax(char *str, t_err_token err_tok);

#endif