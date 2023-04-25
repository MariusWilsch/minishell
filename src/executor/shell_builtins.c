/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:05:40 by tklouwer          #+#    #+#             */
/*   Updated: 2023/04/24 18:19:22 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(int argc, char **argv)
{
	bool	flag;
	int		i;

	i = 1;
	flag = true;
	while (i < argc)
	{
		if (ft_strncmp(argv[i], "-n", 2) == 0 && ft_strlen(argv[i]) == 2)
		{
			i++;
			flag = false;
			continue ;
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

int	cd(int argc, char *path)
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


	chdir(path);


	// if (chdir(path) == -1)
	// 	return (ft_printf("cd: error changing directory: %s\n", path));
	
	return (EXIT_SUCCESS);
}

int	pwd(void)
{
	char	input[1024];
	char	*cwd;

	cwd = getcwd(NULL, 1024);
	if (!cwd)
	{
		perror("getcwd");
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	return (EXIT_SUCCESS);
}

int	sh_exit(int status)
{
	exit(status);
}

int	env(t_env **env_list, bool export)
{
	t_env	*env;



	env = *env_list;
	while (env != NULL)
	{
		if (env->key == NULL || env->value == NULL)
		{
			env = env->next;
			continue ;
		}
		if (export == true)
		{
			ft_printf("declare -x %s=", env->key);
			ft_printf("%s\n", env->value);
			env = env->next;
			continue ;
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
