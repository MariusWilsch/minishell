/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:40:09 by tklouwer          #+#    #+#             */
/*   Updated: 2023/05/30 16:01:47 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	exec_builtin(char *func, int argc, char **argv, t_env **env_list)
{
	if (ft_strcmp("echo", func) == 0)
		echo(argc, argv);
	if (ft_strcmp("cd", func) == 0)
		cd(argc, argv[1], *env_list);
	if (ft_strcmp("pwd", func) == 0)
		pwd();
	if (ft_strcmp("env", func) == 0)
		env(env_list, false);
	if (ft_strcmp("export", func) == 0)
		export(argc, argv, env_list);
	if (ft_strcmp("unset", func) == 0)
		unset(argc, argv, env_list);
	return (EXIT_SUCCESS);
}

int	exisit_env(t_env **env_list, char *str, t_env **found)
{
	char	**key;
	t_env	*temp;
	int		position;

	if (found != NULL)
		*found = NULL;
	key = ft_split(str, '=');
	temp = *env_list;
	position = 0;
	while (temp != NULL)
	{
		if (temp->key != NULL && ft_strcmp(key[0], temp->key) == 0)
		{
			*found = temp;
			free_split(key);
			return (position);
		}
		temp = temp->next;
		position++;
	}
	free_split(key);
	return (-1);
}

// int	export_checks(char *str)
// {
	
// }


int	export(int argc, char *argv[], t_env **env_list)
{
	int		i;
	int		k;
	t_env	*found;
	t_env	*temp;

	i = 1;
	if (argc == 1)
		return (env(env_list, true));
	while (i < argc)
	{
		if (ft_strchr(argv[i], '=') == NULL && exisit_env(env_list, argv[i], &temp) == -1)
		{
			k = 0;
			while (argv[i][k] && ft_isalnum(argv[i][k]))
				k++;
			if (argv[i][k] == '\0')
			{
				add_end(env_list, argv[i], true);
				i++;
				continue ;
			}
		}
		if (ft_strchr(argv[i], '=') == NULL)
		{
			i++;
			continue ;
		}
		if (ft_isdigit(argv[i][0]))
		{
			ft_printf("minishell: export: not a valid identifier\n", argv[i]);
			return (g_status = EXIT_FAILURE);
		}
		exisit_env(env_list, argv[i], &found);
		if (found == NULL)
			add_end(env_list, argv[i], false);
		if (found != NULL)
		{
			free(found->value);
			found->value = ft_strdup(argv[i] + ft_strlen(found->key) + 1);
			if (found->export_only == true)
				found->export_only = false;
		}
		i++;
	}
	return (g_status = EXIT_SUCCESS);
}

void	delete_node(t_env **env_list, int position, t_env *found)
{
	t_env	*temp;
	int		i;

	i = 0;
	temp = *env_list;
	if (position != 0)
	{
		while (i < position - 1 && temp != NULL)
		{
			temp = temp->next;
			i++;
		}
		temp->next = found->next;
	}
	else
		*env_list = found->next;
	free(found->key);
	free(found->value);
	free(found);
}

/**
 * @brief 
 * 
 * 
 * 1. Check if the variable exists in the environment list
 * 2. If it does, figure out the position of the node in the list
 * 3. Delete the node
 */
int	unset(int argc, char *argv[], t_env **env_list)
{
	t_env	*found;
	int		position;

	if (argc == 1)
		return (g_status = EXIT_SUCCESS);
	while (argc > 1)
	{
		if (ft_strchr(argv[argc - 1], '=') != NULL)
		{
			ft_printf("minishell: unset: identifier unvalid\n", argv[argc - 1]);
			return (g_status = EXIT_FAILURE);
		}
		position = exisit_env(env_list, argv[argc - 1], &found);
		if (position == -1)
		{
			return (g_status = EXIT_SUCCESS);
		}
		delete_node(env_list, position, found);
		argc--;
	}
	return (g_status = EXIT_SUCCESS);
}
