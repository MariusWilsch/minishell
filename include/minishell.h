/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 10:38:21 by verdant           #+#    #+#             */
/*   Updated: 2023/03/14 12:27:06 by mwilsch          ###   ########.fr       */
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
 * @brief Command Table
 * 
 * @param cmd_name The name of the command
 */
// typedef struct s_cmds {
// 	char	*tok_str;
// 	enum	tok_type;
// 	// char	*file;
// } t_cmds;

typedef struct s_data {
	int cmd_cnt;
} t_data;

// Helper_1
bool	incl_char(char c, char *search_str);
bool	check_input(const char *str);
int		cnt_occur_skip(char *str, char c, char *skip);


#endif