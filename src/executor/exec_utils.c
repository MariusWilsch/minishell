/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: verdant <verdant@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/29 16:21:11 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/04/25 15:26:34 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	wr_dup2(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
	perror("Dup2");
	exit (EXIT_FAILURE);
}

int	p_error(char *str, int status)
{
	perror(str);
	exit(status);
	return (EXIT_FAILURE);
}

void	execute_command(t_cmds *cmd)
{
	if (cmd->cmd_type == CMD_EXE)
	{
		if (execve(cmd->cmd_path, cmd->argv, convert_data(*cmd->env)) == -1)
			p_error("Execve failed", 127);
	}
	else
		exec_builtin(cmd->cmd_path, cmd->argc, cmd->argv, cmd->env);
}

static void	cleanup_redir(t_redir *redir, int redir_c)
{
	int	i;

	i = 0;
	while (redir_c-- > 0)
	{
		free(redir[redir_c].filename);
		free(redir[redir_c].redirect);
	}
	free(redir);
}

void	cleanup(int cmd_cnt, t_cmds *cmd)
{
	while (cmd_cnt--)
	{
		free(cmd[cmd_cnt].argv);
		if (cmd[cmd_cnt].redir)
			cleanup_redir(cmd[cmd_cnt].redir, cmd[cmd_cnt].redir->redirc);
	}
	free(cmd);
}
