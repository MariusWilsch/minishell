/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_checking.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 12:31:04 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/19 16:18:38 by mwilsch          ###   ########.fr       */
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

// I only need to get the env_var and then check if F_OK and then W_OK
int	env_var_case(char *str, int env_len, int cnt)
{
	char *env_var;
	
	env_var = ft_substr(str, ft_strclen(str, '$'), env_len);
	env_var = sub_env(env_var, env_len - 1);
	if (!env_var)
		return (1);
	if (env_var[0] != '/' && !ft_strchr(str, '\"'))
	{
		ft_printf("minishell: %s: ambiguous redirect\n", env_var);
		free(env_var);
		return (0);
	}
	if (!ft_strchr(str, '\"') && access(env_var, F_OK) == 0)
	{
		ft_printf("minishell: %s: Is a directory\n", env_var);
		return (free(env_var), 1);
	}
	free(env_var);
	while (str[cnt] && str[cnt] == ' ')
		cnt++;
	str = del_substr(del_quotes(str), 0, cnt);
	str = sub_env(str, env_len - 1);
	if (access(str, W_OK))
		return (ft_printf("minishell: %s: permission denied\n", str));
	return (0);
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
	if (!ft_strchr(str, '\'') && ft_strchr(str, '$'))
		return (env_var_case(str, get_env_len(str) + 1, cnt));
	return (0);
}


	// if (ft_strnstr(node->arg, "<>", ft_strlen(node->arg))); // Idk if I need that one
	// 	return