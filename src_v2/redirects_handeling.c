/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_handeling.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 12:07:08 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/14 10:29:20 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

// Assuming if both sides of the redirect are found like <     >
// then we error


int	scan_string(char *str)
{
	int	found;
	int	found2;
	
	if (!str || !str[0])
		return (-1);
	found = ft_search_c(str, '>', '\'');
	found2 = ft_search_c(str, '<', '\'');
	if (found != -1)
		return (found);
	if (found2 != -1)
		return (found2);
	return (-1);
}

/**
 * @brief This is getting messy I need to refactor this code
 * 
 * @note edge case: ls >       >output.txt = error
 * 
 * @note Refactoring complete. BUT I have to implement the logic
 * that multiple redirects have to be accounted for
 * so for example if we have ls >>   a> I'm only getting >>   a 
 * and the 2nd > is missing so far
*/
char *cut_out(char *str, int start, t_data *data)
{
	char *res;
	int	len;

	data->redir_cnt = 0;
	len = start;
	if (!str)
		return (NULL);
	while (str[len] && incl_char(str[len], " ><"))
		len++;
	while (str[len] && !incl_char(str[len], " ><"))
		len++;
	len -= start;
	res = ft_substr(str, start, len);
	if (!res)
		return (NULL);
	while (res[data->redir_cnt] && res[data->redir_cnt] == res[0])
		data->redir_cnt++;
	return (res);
}

/**
 * @note 2. while loop checks if there is sth like >      >
 * @note 3. while loop checks if there is an alnum value
 * @note Missing functionality: correct error msg. Example: > or >> when
 * cnt = 3 or cnt = 4 respectively
 * @note I need to find a different way to send the error messages
 * I was thinking that I could build a wrapper around ft_printf and send the 
 * correct msg then 
*/
bool	check_syntax(char *str, char c, int cnt, t_data *data)
{
	int	i;

	// printf("|%s|\n", str);
	i = cnt - 1;
	if (cnt > 2) // minishell: syntax error near unexpected token `%c'\n
		return (error_syntax(NULL, TOO_MANY));
	if (ft_search_c(str, '$', '\'') != -1 && !get_env(str, data))
	{
		return (error_syntax(NULL, AMBIGOUS_REDIRECT));
	}
	while (str[i])
	{
		i++;
		if (str[i] && ft_isalnum(str[i]))
			break;
		if (incl_char(str[i], "><"))
			return (error_syntax(NULL, NO_ALNUM_1));
	}
	i = 0;
	while (str[i])
	{
		if (ft_isalnum(str[i]))
			break ;
		i++;
		if (str[i] == '\0')
			return (error_syntax(NULL, NEWLINE)); // minishell: syntax error near unexpected token `newline'
	}
	return (true);
}

/**
 * @brief Semantic meaning of check_sematics
 * 
 * @param symbol is the redirection character itself
 * @note this part needs to be refactored and then put into the executor
 * data->fd = open(str, O_CREAT | O_WRONLY | O_TRUNC, 0644) for >
 * data->fd = open(str, O_CREAT | O_WRONLY | O_APPEND, 0644) for >>
*/
bool	check_sematics(char *str, char symbol, int cnt, t_data *data)
{
	int	spc_len;
	
	spc_len = 0;
	// Earsing Spaces between redirect and red_arg
	while (str[spc_len + cnt] && incl_char(str[spc_len + cnt], " "))
		spc_len++;
	// Change if I refactor del_substr to a bool func
	str = del_substr(str, 0, cnt + spc_len);
	// Guard Clauses Technique
		// Is this the right place for the fd?
	if (symbol == '>' && cnt == 1)
		return (true);
	if (symbol == '>' && cnt == 2)
		return (true);
	// Symbol either '<'
	if (symbol == '<' && cnt == 1 && access(str, F_OK) == -1)
		return (error_syntax(str, NO_FILE)); // bash: test: No such file or directory
	return (true);
}


/**
 * @brief redirect main wrapper
 * 
 * @note Things left to do
 * - redirect inside of string literal
 * - What happens after I'm done with all of this
*/
bool	redirect_pars(char *str, t_data *data)
{
	char *cutout;
	char *dup;
	int	found;
	
	found = scan_string(str);
	if (!str[0] || found == -1)
		return (true);
	dup = ft_strdup(str);
	while (true)
	{
		cutout = cut_out(dup, found, data);
		printf("|%s|\n", cutout);
		if (!cutout || !check_syntax(cutout, cutout[0], data->redir_cnt, data))
			return (false);
		dup = del_substr(dup, ft_strclen(dup, cutout[0]), ft_strlen(cutout));
		// if (!check_sematics(cutout, cutout[0], data->redir_cnt, data))
		// 	return (false); // This will go to the executor
		free(cutout);
		found = scan_string(dup);
		if (found == -1)
			return (free(dup), true);
	}
}

