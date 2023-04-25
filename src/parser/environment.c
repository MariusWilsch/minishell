/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 17:43:37 by verdant           #+#    #+#             */
/*   Updated: 2023/04/25 16:39:16 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**convert_data(t_env *env)
{
	t_env		*next;
	char		*temp;
	char		**shell_envp;
	int			i;

	i = 0;
	shell_envp = (char **)malloc(sizeof(char *) * (ft_lstsize_shell(env) + 1));
	if (!shell_envp)
		return (NULL);
	shell_envp[i] = NULL;
	while (env)
	{
		shell_envp[i] = ft_strjoin(env->key, "=");
		temp = shell_envp[i];
		shell_envp[i] = ft_strjoin(shell_envp[i], env->value);
		next = env->next;
		free(temp);
		free(env->key);
		free(env->value);
		free(env);
		env = next;
		i++;
	}
	return (shell_envp);
}

t_env	*get_key_value(t_env *node, char *str)
{
	int	key_len;
	int	i;
	int	j;

	i = 0;
	j = 0;
	key_len = ft_strclen(str, '=');
	node->key = (char *)malloc(sizeof(char) * key_len + 1);
	node->value = (char *)malloc(sizeof(char) * (ft_strlen(str) - key_len));
	if (!node->key || !node->value)
		return (NULL);
	while (str[i] && i < key_len)
	{
		node->key[i] = str[i];
		i++;
	}
	node->key[i] = '\0';
	if (str[i] == '=')
		i++;
	while (str[i] && str[i] != '\0')
		node->value[j++] = str[i++];
	node->value[j] = '\0';
	return (node);
}

void	add_end(t_env **head, char *str)
{
	t_env	*temp;
	t_env	*new_node;

	temp = *head;
	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->next = NULL;
	if (ft_strchr(str, '='))
		new_node = get_key_value(new_node, str);
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new_node;
}

void	env_init(t_env **env, char **envp)
{
	int		index;

	if (*env != NULL)
		return ;
	index = 0;
	while (envp[index])
	{
		add_end(env, envp[index]);
		index++;
	}
}
