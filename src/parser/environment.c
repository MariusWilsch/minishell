/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 17:43:37 by verdant           #+#    #+#             */
/*   Updated: 2023/04/19 14:32:48 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// void print_list_env(t_env *env)
// {
// 	while (env)
// 	{
// 		printf("%s=", env->key);
// 		printf("%s\n", env->value);
// 		// printf("hidden: %d", env->hidden);
// 		env = env->next;
// 	}
// }

// void print_arr(char **arr)
// {
// 	int i = 0;
// 	puts("Printing array: \n");
// 	while (arr[i])
// 	{
// 		printf("%s\n", arr[i]);
// 		i++;
// 	}
// }

#include "minishell.h"

char	**convert_data(t_env *env)
{
	t_env		*next;
	char		**shell_envp;
	int			i;
	
	i = ft_lstsize_shell(env);
	shell_envp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!shell_envp)
		return (NULL);
	shell_envp[i] = NULL;
	i = 0;
	while (env)
	{
		next = env->next;
		shell_envp[i] = ft_strjoin(env->key, "=");
		shell_envp[i] = ft_strjoin(shell_envp[i], env->value);
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
	int	i;
	int	j;

	i = 0;
	j = 0;
	const int key_len = ft_strclen(str, '=');
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
	node->hidden = false;
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
	else
	{
		new_node->key = ft_strdup(str);
		new_node->value = ft_strdup("");
		new_node->hidden = true;
	}
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new_node;
}

void env_init(t_env **env, char **envp)
{
		int		index;
		t_env	*temp;
		
		if (*envp != NULL)
			return ;
		index = 0;
		while (envp[index])
		{
			temp = *env;
			add_end(env, envp[index]);
			index++;
		}
}
