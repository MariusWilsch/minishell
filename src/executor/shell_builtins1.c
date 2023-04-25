/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builtins1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:40:09 by tklouwer          #+#    #+#             */
/*   Updated: 2023/04/25 14:02:48 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @param env_list 
 */
int	export(int argc, char *argv[], t_env **env_list)
{
	int		i;
	t_env	*temp;

	i = 1;
	if (argc == 1)
		return (env(env_list, true));
	while (i < argc)
	{
		// if (ft_strchr(argv[i], ' ') != NULL)
		// 	i++;
		temp = exisit_env(env_list, argv[i]);
		if (temp == NULL)
			add_end(env_list, argv[i]);
		else if (temp != NULL && ft_strcmp(argv[i], temp->value) != 0)
		{
			temp->hidden = false;
			free(temp->key);
			free(temp->value);
			get_key_value(temp, argv[i]);
		}
		else if (temp != NULL && temp->hidden == true)
			temp->hidden = false;
		i++;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief 
 * 
 * 
 * 1. If key exists set value to new value
 * 2. If key does not exist add new key value pair
 * 3. If key exisit then delete the node
 */
int	unset(int argc, char *argv[], t_env **env_list)
{
	int		i;
	t_env	*temp;

	i = 1;
	if (argc == 1)
		return (EXIT_SUCCESS);
	
	while (i < argc)
	{
		temp = exisit_env(env_list, argv[i]);
		if (temp)
			temp->hidden = true;
		i++;
	}
	return (EXIT_SUCCESS);
}

t_env	*exisit_env(t_env **env_list, char *str)
{
	char	**key;
	t_env	*temp;

	key = ft_split(str, '=');
	temp = *env_list;
	while (temp != NULL)
	{
		if (temp->key != NULL && ft_strncmp(key[0], temp->key,
				ft_strlen(*key)) == 0)
			return (temp);
		temp = temp->next;
	}
	free_split(key);
	free(temp);
	return (NULL);
}
