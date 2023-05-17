/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_builtins_utils.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/17 16:21:38 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/05/17 16:22:26 by tklouwer      ########   odam.nl         */
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

int	exec_builtin(char *func, int argc, char **argv, t_env **env_list)
{
	if (ft_strcmp("echo", func) == 0)
		echo(argc, argv);
	if (ft_strcmp("cd", func) == 0)
		cd(argc, argv[1], *env_list);
	if (ft_strcmp("pwd", func) == 0)
		pwd();
	if (ft_strcmp("env", func) == 0)
		env(env_list);
	if (ft_strcmp("export", func) == 0)
		export(argc, argv, env_list);
	if (ft_strcmp("unset", func) == 0)
		unset(argc, argv, env_list);
	return (EXIT_SUCCESS);
}

void shell_exit(t_cmds *cmd) 
{
	if (count_args(cmd->argv) > 2)
	{
		ft_printf("exit: too many arguments\n");
		exit(1);
	}
	rl_clear_history();
	exit(ft_atoi(cmd->argv[1]));
}
