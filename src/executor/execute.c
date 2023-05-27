/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwilsch <mwilsch@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/22 16:40:17 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/05/27 10:14:44 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <sys/wait.h>

void  parent_process(pid_t child_pid)
{
	int status;

	if (waitpid(child_pid, &status, 0) < 0)
	{
		if (errno != ECHILD)
		p_error("waitpid", 1);
	}
	if (WIFEXITED(status))
		g_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		g_status = 130;
	}
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			ft_putstr_fd("Quit: 3\n", 1);
			g_status = 131;
		}
	}
}

int	child_process(t_cmds *cmd, int i, int cmd_cnt, int *pipe_fd)
{
	int	heredoc_fd;

	heredoc_fd = -1;
	signal(SIGQUIT, child_signal_handler);
	signal(SIGINT, child_signal_handler);
	handle_heredoc(cmd, &heredoc_fd);
	close_pipes(pipe_fd, cmd_cnt, i, 0);
	if (cmd_cnt != 1 && !cmd->redir)
		redirect_pipe_fd(i, cmd_cnt, pipe_fd);
	if (cmd->redir)
		process_redirection(cmd, i, pipe_fd);
	execute_cmd_or_builtin(cmd, i);
	exit(EXIT_SUCCESS);
}

void	exec_pipeline(t_cmds *cmd, int cmd_cnt, int *pipe_fd, pid_t *pid)
{
	int	i;

	i = 0;
	while (i < cmd_cnt)
	{
		pid[i] = fork();
		if (pid[i] < 0)
			p_error("fork", 1);
		else if (pid[i] == 0)
		{
			child_process(cmd, i, cmd_cnt, pipe_fd);
		}
		i++;
	}
}

void	shell_process(t_cmds *cmd, int cmd_cnt, int *pipe_fd)
{
	pid_t	*pid;
	int		i;

	i = 0;
	pid = malloc(sizeof(pid_t) * cmd_cnt);
	if (pid == NULL)
		p_error("malloc", EXIT_FAILURE);
	exec_pipeline(cmd, cmd_cnt, pipe_fd, pid);
	close_pipes(pipe_fd, cmd_cnt, i, 1);
	while (i < cmd_cnt)
	{
		parent_process(pid[i]);
		i++;
	}
	free(pid);
}

/* RESPONSIBLE FOR SETTING UP THE NECESSARY STRUCTURES FOR HANDLING COMMANDS
	AND MANAGING THE CHILD PROCESSES.
	- CMND_CNT = NUMBER OF COMMANDS.
	- PIPE_FD = ARRAY OF FILE DESCRIPTORS FOR THE PIPES
	- BUILT-INS NEED TO BE EXECUTED IN THE PARENT PROCESS. SO WE DONT FORK THEM. 
 */
int	executor(t_args *head, t_env **env_l)
{
	t_cmds			*cmd;
	int				cmd_cnt;
	int				*pipe_fd;

	cmd_cnt = 1;
	cmd = create_structs(head, &cmd_cnt, env_l);
	if (ft_strcmp("exit", head->arg) == 0)
	{
		if (mini_exit(cmd))
			return (EXIT_FAILURE);
	}
	if (cmd->cmd_type == BUILT_IN_EXE && cmd_cnt == 1 &&
		ft_strcmp("echo", head->arg) != 0)
		return (exec_builtin(cmd->cmd_path, cmd->argc, cmd->argv, cmd->env));
	if (cmd_cnt > 1)
		pipe_fd = create_pipes(cmd_cnt);
	shell_process(cmd, cmd_cnt, pipe_fd);
	cleanup(cmd_cnt, cmd, pipe_fd);
	return (EXIT_SUCCESS);
}
