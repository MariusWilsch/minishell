/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   process_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/22 11:12:21 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/05/23 16:05:35 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	process_redirection(t_cmds *cmd, int i, int *pipe_fd)
{
	int heredoc_fd = -1;
	
	handle_heredoc(cmd, &heredoc_fd);
	if (heredoc_fd >= 0)
	{
		if (close(heredoc_fd) < 0)
			p_error("close", EXIT_FAILURE);
	}
	if (cmd[i].redir)
	{
		if (!heredoc_fd)
			cmd[i].in_fd = pipe_fd[0];
		cmd[i].out_fd = pipe_fd[1];
		redirect_command_fd(&cmd[i]);
	}
}

void  execute_cmd_or_builtin(t_cmds *cmd)
{
	if (cmd->cmd_type == BUILT_IN_EXE)
		exec_builtin(cmd->cmd_path, cmd->argc, cmd->argv, cmd->env);
	else
		execute_command(cmd);
}

void	open_pipes(t_cmds *cmd, int cmd_cnt, int *pipe_fd)
{
	int	i;

	i = 0;
	cmd[0].in_fd = -1;
	while (i < cmd_cnt)
	{
		if (pipe(pipe_fd + i * 2))
			p_error("pipe", 1);
		cmd[i + 1].in_fd = pipe_fd[2 * i];
		cmd[i].out_fd = pipe_fd[2 * i + 1];
		i++;
	}
}
