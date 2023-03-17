/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 18:12:44 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/17 17:40:02 by mwilsch          ###   ########.fr       */
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
	free(str);
	if (!temp)
		return (NULL);
	return (temp);
}

void	*cmd_err(t_args *node) // Rename this later
{
	char *str = node->arg;
	if (ft_strchr(str, '$'))
	{
		str = env_res(str, get_env_len(str)); // Leak?
		ft_printf("minishell: %s: No such file or directory\n", str);
		node->err_tok = NO_FILE_DIR;
		return (str);
	}
	ft_printf("minishell: %s: command not found\n", str);
	node->err_tok = NO_CMD;
	return (str);
}


char	*resolute_cmd(t_args *node, char *str)
{
	char *temp;
	char **path_2D;
	int	i;

	i = 0;
	temp = prep_cmd(str); // Malloc
	path_2D = ft_split(getenv("PATH"), ':');  // Malloc
	while (path_2D && temp && path_2D[i] != NULL)
	{
		str = ft_strjoin(path_2D[i], temp); // Malloc Loop
		if (!str)
			return (free_split(path_2D), str);
		if (access(str, X_OK) == 0)
			break ;
		i++;
		free(str);
		str = NULL;
	}
	free(temp);
	free_split(path_2D);
	if (str == NULL)
		return (free(str), cmd_err(node));
	return (free(node->arg), str);
}



/**
 * @brief counts the length of the unsubsituted env_var
 * 
 * @return e.G $HOME it returns 4 (4 chars after the $)
*/
int	get_env_len(char *str)
{
	const	int start = ft_strclen(str, '$') + 1;
	int	len;

	len = start;
	while (str[len] && !incl_char(str[len], " <>\""))
		len++;
	return (len - start);
}

/**
 * @brief Substitutes the environment variable inside of of the string
 * 
 * @param str String to be subsitutued. str gets freed!
 * @param env_len The length of the unsubsituted env_var
 * 
 * @note First it copies all chars before the $. Then it concatenates the env_var.
 * Finally it copies to the rest of the string
 * 
 * @note I'm missing the deletion of env_var if env_var does not exsit
*/	
char	*env_res(char *str, int env_len)
{
	const int	start = ft_strclen(str, '$') + 1;
	char			*env_var;
	char			*new;
	int				len;

	new = ft_substr(str, start, env_len);
	env_var = getenv(new);
	if (!env_var)
		return (NULL);
	len = (ft_strlen(str) + ft_strlen(env_var)) - (env_len + 1);
	free(new);
	new = malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	new[len] = '\0';
	len = ft_strlcpy(new, str, ft_strclen(str, '$') + 1);
	len = ft_strlcat(new, env_var, len + ft_strlen(env_var) + 1);
	env_len += start;
	while (str[env_len] && env_len < len)
		new[len++] = str[env_len++];
	free(str);
	return (new);
}