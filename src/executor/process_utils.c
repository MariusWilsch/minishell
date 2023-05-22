/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   process_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/22 11:12:21 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/05/22 14:22:58 by dickklouwer   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	process_redirection(t_cmds *cmd, int i, int heredoc_fd, int *pipe_fd)
{
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

void  execute_cmd_or_builtin(t_cmds *cmd, int i)
{
  if (cmd->cmd_type == BUILT_IN_EXE)
    exec_builtin(cmd->cmd_path, cmd->argc, cmd->argv, cmd->env);
  else
    execute_command(&cmd[i]);
}

int	*create_pipes(int cmd_cnt)
{
	int	*pipe_fd;
	int	i;

	i = 0;
	pipe_fd = (int *)calloc(2 * cmd_cnt, sizeof(int));
	if (pipe_fd == NULL)
		p_error("calloc", EXIT_FAILURE);
	while (i < cmd_cnt)
	{
		if (pipe(pipe_fd + 2 * i) < 0)
			p_error("pipe", EXIT_FAILURE);
		i++;
	}
	return (pipe_fd);
}
