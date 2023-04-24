/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:05:40 by tklouwer          #+#    #+#             */
/*   Updated: 2023/04/19 14:31:10 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>


int echo(int argc, char **argv)
{
		bool	flag;
		int i;

		i = 1;
		flag = true;
		while (i < argc)
		{
				if (ft_strncmp(argv[i], "-n", 2) == 0 && ft_strlen(argv[i]) == 2)
				{
						i++;
						flag = false;
						continue;
				}
				ft_printf("%s", argv[i]);
				if (i != argc - 1)
						ft_printf(" ");
				i++;
		}
		if (flag)
				ft_printf("\n");
		return (i);
}

int cd(int argc, char *path)
{
	if (argc > 2)
		return (ft_printf("cd: too many arguments\n", 2));
	if (ft_strncmp(path, "..", 2) == 0 || ft_strncmp(path, ".", 1) == 0)
	{
		if (chdir(path) == -1)
			return (ft_printf("cd: error changing directory: %s\n", path));
		return (EXIT_SUCCESS);
	}
	if (access(path, F_OK) == -1)
		return (ft_printf("cd: no such file or directory: %s\n", path));
	if (access(path, X_OK) == -1)
		return (ft_printf("cd: permission denied: %s\n", path));
	if (chdir(path) == -1)
		return (ft_printf("cd: error changing directory: %s\n", path));
	return (EXIT_SUCCESS);
}

int pwd(void)
{
		char input[1024];
		char *cwd;

		cwd = getcwd(input, 1024);
		if (!cwd)
		{
				perror("getcwd");
				return (1);
		}
		ft_putendl_fd(cwd, 1);
		return (EXIT_SUCCESS);
}

int sh_exit(int status)
{
		exit(status);
}

int env(t_env **env_list, bool export)
{
	t_env*	env;

	env = *env_list;
	while (env != NULL)
	{
		if (env->key == NULL || env->value == NULL)
		{
			env = env->next;
			continue;
		}
		if (export == true)
		{
			ft_printf("declare -x %s=", env->key);
			ft_printf("%s\n", env->value);
			env = env->next;
			continue;
		}
		if (env->hidden == false)
		{
			ft_printf("%s=", env->key);
			ft_printf("%s\n", env->value);
		}
		env = env->next;
	}
	return (EXIT_SUCCESS);
}


t_env	*exisit_env(t_env **env_list, char *str)
{
	char **key;
	t_env *temp;
	int len;


	key = ft_split(str, '=');
	temp = *env_list;
	while (temp != NULL)
	{
		if (temp->key != NULL && ft_strcmp(key[0], temp->key) == 0)
			return (temp);
		temp = temp->next;
	}
	free_split(key);
	free(temp);
	return (NULL);
}

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @param env_list 
 * 
 * 
 * @note REFACTOR IDEA: If I would have a circular list I could just check from the back to the front which would make it faster
 */
int	export(int argc, char *argv[], t_env **env_list)
{
	int i;
	t_env *temp;

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
 * 	2.1 It depends on if the hidden method works properly. It's kind of a hacky way to do it
 * 
 * The problem is that 
 */
int unset(int argc, char *argv[], t_env **env_list)
{
	int i;
	t_env *temp;

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
