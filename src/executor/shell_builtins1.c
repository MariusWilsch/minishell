/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_builtins1.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/24 10:40:09 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/04/24 10:42:41 by tklouwer      ########   odam.nl         */
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
		temp = exisit_env(env_list, argv[i]);
		if (temp == NULL)
			add_end(env_list, argv[i]);
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
 * 1. Check if the variable exists
 * 2. If it does, change the env.hidden = true or delete the node
 * 	2.1 It depends on if the hidden method works properly.
 *  It's kind of a hacky way to do it
 * 
 * The problem is that 
 */
int	unset(int argc, char *argv[], t_env **env_list)
{
	int		i;
	t_env	*temp;

	i = 1;
	if (argc == 1)
		return (EXIT_SUCCESS);
	if (i < argc)
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
	int		len;
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
