/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_checking.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 12:31:04 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/18 17:57:22 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	err_msg(t_err_tok err, char *str)
{
	if (err == TO_MANY)
		return (ft_printf("minshell: parse error: To many consecutive redirects\n"));
	if (err == NO_ALNUM_BETWEEN)
		return (ft_printf("minshell: parse error: No alnum value between redirect\n"));
	if (err == NEWLINE_ERR)
		return (ft_printf("minshell: parse error: No alnum value after redirect\n"));
		
	return (2132311232);
}

/**
 * @brief Checking the syntax of the redirects
 * 
 * @note Possible errors
 * 0. To many: >>> or <<< (3+ redirects)
 * 1. Ambigous Redirect: >$HOE
 * 2. No alnum value in between redirects: >   >file
 * 3. No alnum value after redirect: cmd >
 * 4. Special case: <> is valid but I will error
*/

int	check_redirect(char *str, char c)
{
	int	i;
	int	cnt;

	cnt = 0;
	if (!str)
		return (2);
	while (str[cnt] && str[cnt] == c)
		cnt++;
	i = cnt - 1;
	if (cnt > 2)
		return (err_msg(TO_MANY, NULL));
	while (str[i])
	{
		if (ft_isalnum(str[i]))
			break ; // could this be a return ;?
		i++;
		if (incl_char(str[i], "><"))
			return (err_msg(NO_ALNUM_BETWEEN, NULL));
		if (str[i] == '\0')
			return (err_msg(NEWLINE_ERR, NULL));
	}


	
	// this is for 
		// 1. bash: /Users/mwilsch: Is a directory (when >$HOME)
		// 2. bash: /Users/mwilsch test: Permission denied (when for example >"$HOME test")
	// but this is frankenstein code and I need refactor this or think what could be done better
	if (!ft_strchr(str, '\'') && ft_strchr(str, '$'))
	{
		str = sub_env(ft_strdup(str), get_env_len(str));
		if (!ft_strchr(str, '\"'))
		{
			str = del_substr(str, 0, cnt);
			if (access(str, F_OK) == 0)
				ft_printf("%s: Is a directory", str);
			return (1);
		}
		str = ft_substr(str, ft_strclen(str, '\"') + 1, cnt_len_between(str, '\"', ft_strclen(str, '\"') + 1));
		if (access(str, R_OK) == -1)
			ft_printf("%s: Permission Denied", str);
	}
	return (0);
}


	// if (ft_strnstr(node->arg, "<>", ft_strlen(node->arg))); // Idk if I need that one
	// 	return