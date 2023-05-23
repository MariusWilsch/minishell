/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwilsch <mwilsch@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/29 16:21:11 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/05/23 16:20:11 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void  execute_command(t_cmds *cmd)
{
	t_env *found;
	int   shlvl;

	shlvl = 1;
	if (cmd->cmd_path == NULL)
		exit(127);
	if (exisit_env(cmd->env, "PATH", &found) == -1)
	{
		ft_printf("minishell: %s: No such file or directory\n",
		cmd->cmd_path);
		exit(127);
	}
	if (ft_strcmp(cmd->cmd_path, "./minishell") == 0)
	{
    	if (exisit_env(cmd->env, "SHLVL", &found) > 0)
    	{
			shlvl = ft_atoi(found->value) + 1;
			free(found->value);
			found->value = ft_itoa(shlvl);
		}
		if (execve(cmd->cmd_path, cmd->argv, convert_data(*cmd->env)) == -1)
			exit(127);
	}
	else if (execve(cmd->cmd_path, cmd->argv, NULL) == -1)
		exit(127);
}

int	wr_dup2(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
	perror("Dup2");
	exit (g_status);
}

int	p_error(char *str, int status)
{
	perror(str);
	exit(status);
}

int	count_args(char **argv)
{
	int	count;

	count = 0;
	while (argv[count] != NULL)
	{
		count++;
	}
	return (count);
}

void	cleanup(int cmd_cnt, t_cmds *cmd, int *pipe_fd)
{
	int	red_cnt;

	while (cmd_cnt--)
	{
		free(cmd[cmd_cnt].argv);
		red_cnt = cmd[cmd_cnt].redircnt;
		if (red_cnt > 0)
		{
			while (red_cnt--)
				free(cmd[cmd_cnt].redir[red_cnt].filename);
			free(cmd[cmd_cnt].redir);
		}
	}
	if (cmd_cnt > 1)
		free(pipe_fd);
	free(cmd);
}
