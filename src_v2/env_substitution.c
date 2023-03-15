/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_substitution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 20:40:10 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/15 17:06:05 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

int	copy_before(char **temp, char *str, t_data *data)
{
	int i;
	int	cnt;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			cnt++;
		if (str[i] == '$' && ((cnt % 2) == 0))
			break ;
		(*temp)[i] = str[i];
		i++;
	}
	return (i);
}

char	*get_env(char *str, t_data *data)
{
	char	*env_var;
	int		size;
	int		i;

	data->env_start = ft_search_c(str, '$', '\'') + 1;
	i = data->env_start;
	while (str[i] && !incl_char(str[i], " <>\'\""))
	{
		data->env_len++;
		i++;
	}
	env_var = getenv(ft_substr(str, data->env_start, data->env_len));
	if (!env_var)
		return (NULL);
	size = ft_strlen(str) + ft_strlen(env_var);
	data->env_size = size - (data->env_len + 1);
	return (env_var);
}

char	*substitute_var(char *str, char *env_var, int env_len, t_data *data)
{
	char				*temp;
	int					i;
	int					k;
	int					temp_i;

	i = 0;
	k = 0;
	if (!env_var)
		return (del_substr(str, data->env_start - 1, data->env_len + 1));
	temp = malloc(sizeof(char) * (data->env_size + 1));
	if (!temp || !str)
		return (NULL);
	temp[data->env_size] = '\0';
	i += copy_before(&temp, str, data);
	temp_i = env_len + i; 
	while (env_var[k])
		temp[i++] = env_var[k++];
	while (str[temp_i])
		temp[i++] = str[temp_i++];
	free(str);
	return (temp);
}

bool	env_res(char **str, t_data *data)
{
	const int	cnt = count_occurences_skip(*str, '$', "\'");
	int				i;

	i = 0;
	// Checking if Env Var exits
	if (*str[0] == '\0' || cnt <= 0)
		return (true);
	data->env_size = 0;
	data->env_len = 0;
	while (i < cnt)
	{
		*str = substitute_var(*str, get_env(*str, data), data->env_len + 1, data);
		if (!*str)
			return (false);
		data->env_len = 0;
		i++;
	}
	return (true);
}
