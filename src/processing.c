/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 18:12:44 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/17 11:39:51 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *prep_cmd(char *str)
{
	char *temp;
	int	i;

	i = 1;
	if (str[0] == '|')
	{
		while (str[i] && str[i] == ' ')
			i++;
		str = del_substr(str, 0, i);
	}
	temp = ft_strjoin("/", str);
	if (!temp)
		return (NULL);
	free(str);
	return (temp);
}

char	*resolute_cmd(char *str)
{
	const char *path = getenv("PATH");
	char *temp;
	char **path_2D;
	int	i;

	i = 0;
	str = prep_cmd(str); // Malloc
	if (!str || !path)
		return (NULL);
	path_2D = ft_split(path, ':');  // Malloc
	while (path_2D[i] != NULL)
	{
		temp = ft_strjoin(path_2D[i], str); // Malloc Loop
		if (!temp)
			return (free_split(path_2D), NULL);
		if (access(temp, X_OK) == 0)
			break ;
		free(temp);
		i++;
	}
	free(str);
	free_split(path_2D);
	return (temp);
}