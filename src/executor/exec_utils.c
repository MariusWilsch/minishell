/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwilsch <mwilsch@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/29 16:21:11 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/05/20 12:30:52 by dickklouwer   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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

// void	cleanup(int cmd_cnt, t_cmds *cmd, int *pipe_fd)
// {
// 	while (cmd_cnt--)
// 	{
// 		free(cmd[cmd_cnt].argv);
// 		if (cmd[cmd_cnt].redir)
// 		{
// 			while (cmd[cmd_cnt].redircnt--)
// 				free(cmd[cmd_cnt].redir[cmd_cnt].filename);
// 			free(cmd->redir);
// 		}
// 	}
// 	free(pipe_fd);
// 	free(cmd);
// }

void	execute_command(t_cmds *cmd)
{
	t_env	*found;
	int		shlvl;

	shlvl = 1;
	if (cmd->cmd_type == CMD_EXE)
	{
		if (exisit_env(cmd->env, "PATH", &found) == -1)
		{
			ft_printf("minishell: %s: No such file or dir\n", cmd->cmd_path);
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
}
