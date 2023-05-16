/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_builtins.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwilsch <mwilsch@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/07 14:05:40 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/05/16 15:05:54 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	check_flag(char *argv[])
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (ft_strncmp(argv[i], "-n", 2) == 0 && ft_strlen(argv[i]) == 2)
			return (false);
		i++;
	}
	return (true);
}

int	echo(int argc, char **argv)
{
	int		i;

	i = 1;
	while (i < argc)
	{
		if (ft_strcmp(argv[i], "$?") == 0)
		{
			ft_putnbr(g_status);
			i++;
			continue ;
		}
		if (ft_strcmp(argv[i], "-n") == 0)
		{
			i++;
			continue ;
		}
		ft_printf("%s", argv[i]);
		if (i != argc - 1)
			ft_printf(" ");
		i++;
	}
	if (check_flag(argv) == true)
		ft_printf("\n");
	return (g_status = EXIT_SUCCESS);
}

int	cd(int argc, char *path, t_env *env_list)
{	
	t_env	*temp;

	if (argc == 1)
	{
		if (exisit_env(&env_list, "HOME", &temp) == -1)
			return (g_status = 1, ft_printf("cd: HOME not set\n", 2));
		return (chdir(temp->key));
	}
	if (argc > 2)
		return (g_status = 1, ft_printf("cd: too many arguments\n", 2));
	if (ft_strncmp(path, "..", 2) == 0 || ft_strncmp(path, ".", 1) == 0)
	{
		if (chdir(path) == -1)
			return (g_status = 1,
				ft_printf("cd: error changing directory: %s\n", path));
		return (g_status = EXIT_SUCCESS);
	}
	if (access(path, F_OK) == -1)
		return (g_status = 1,
			ft_printf("cd: no such file or directory: %s\n", path));
	if (access(path, X_OK) == -1)
		return (g_status = 1, ft_printf("cd: permission denied: %s\n", path));
	chdir(path);
	return (g_status = EXIT_SUCCESS);
}

int	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 1024);
	if (!cwd)
	{
		perror("getcwd");
		return (g_status = EXIT_FAILURE);
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (g_status = EXIT_SUCCESS);
}

int	env(t_env **env_list)
{
	t_env	*env;

	env = *env_list;
	while (env)
	{
		ft_printf("%s=", env->key);
		ft_printf("%s\n", env->value);
		env = env->next;
	}
	return (g_status = EXIT_SUCCESS);
}
