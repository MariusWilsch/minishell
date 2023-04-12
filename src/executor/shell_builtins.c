/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:05:40 by tklouwer          #+#    #+#             */
/*   Updated: 2023/04/12 13:33:04 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



/**
 * @brief Echo command
 * 
 * @param argc 
 * @param argv 
 */
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

int env()
{    
		extern char **environ;
		char **envp;

		envp = environ;
		while (*envp != NULL) 
		{
				printf("%s\n", *envp);
				envp++;
		}
		envp = NULL;
		return (EXIT_SUCCESS);
}


/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */

// 1. Check if unset has arguments
// 2. Count the env_vars in environ
// 3. Check if argv[i] is in environ
// 4. Check if argv[i] is in the the last element
// 5. If not, move the rest of the elements up one


bool pop_element(char **environ,const int env_cnt, char *str)
{
	int	i;
	
	i = 0;
	if (environ[env_cnt] == str)
	{
		puts("I'm here");
		environ[env_cnt] = NULL;
		return (true);
	}
	// while (environ[i] != str)
	// 	i++;
	// while (i < env_cnt && environ[i + 1] != NULL)
	// {
	// 	environ[i] = environ[i + 1];
	// 	i++;
	// }
	// environ[i] = NULL;
	// for (i = 0; environ[i] != NULL; i++)
	// 	ft_printf("%s", environ[i]);
	return(true);
}


int	unset(int argc, char *argv[])
{

	int	i;
	int	env_cnt;

	i = 1;
	env_cnt = 0;
	if (argc == 1)
		ft_printf("unset: not enough arguments\n");
	while (environ[env_cnt] != NULL)
		env_cnt++;
	while (i < argc)
	{
		if (ft_strchr(argv[i], ' ') != NULL)
			ft_printf("unset: not a valid identifier: %s\n", argv[i++]);
		if (getenv(argv[i]) != NULL && !pop_element(environ, env_cnt, argv[i]))
			ft_printf("unset: pop_element error\n");
		i++;
	}
	return (EXIT_SUCCESS);
}

			// if (environ[env_cnt] == argv[i]) // If the last element of environ is the same as argv[i] then just set it to NULL
			// 	environ[env_cnt] = NULL;
			// else
			// {
				
			// }
