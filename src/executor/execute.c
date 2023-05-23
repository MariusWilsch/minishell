/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwilsch <mwilsch@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/22 16:40:17 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/05/23 16:23:36 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <sys/wait.h>

void	execute_pipeline(t_cmds *cmd, int cmd_cnt, int *pipe_fd, pid_t *pid)
{
	int	i;

	i = 0;
	while (i < cmd_cnt)
	{
		*pid = fork();
		if (*pid == 0)
			child_process(cmd, i, cmd_cnt, pipe_fd);
		else if (*pid == -1)
			p_error("fork", EXIT_FAILURE);
		i++;
	}
	parent_process(*pid);
	if (*pid != -1)
	{
		waitpid(*pid, &g_status, 0);
		if (WIFEXITED(g_status))
			g_status = WEXITSTATUS(g_status);
	}
}

void	close_file_descriptors(int *pipe_fd, int cmd_cnt)
{
	int	i;

	i = 0;
	while (i < 2 * cmd_cnt)
	{
		close(pipe_fd[i]);
		i++;
	}
}

int	child_process(t_cmds *cmd, int i, int cmd_cnt, int *pipe_fd)
{
	signal(SIGQUIT, child_signal_handler);
	signal(SIGINT, child_signal_handler);
	if (i > 0)
	{
		if (dup2(pipe_fd[2 * (i - 1)], STDIN_FILENO) == -1)
			p_error("dup2", EXIT_FAILURE);
	}
	if (i < cmd_cnt - 1)
	{
		if (dup2(pipe_fd[2 * i + 1], STDOUT_FILENO) == -1)
			p_error("dup2", EXIT_FAILURE);
	}
	close_file_descriptors(pipe_fd, cmd_cnt);
	if (cmd->redir)
		process_redirection(cmd, i, pipe_fd);
	if (cmd->cmd_type == BUILT_IN_EXE)
		exec_builtin(cmd->cmd_path, cmd->argc, cmd->argv, cmd->env);
	else
		execute_command(cmd);
	exit (EXIT_SUCCESS);
}

/* EXECUTES THE ACTIONS THAT NEEDS TO BE PERFORMED FOR THE PARENT PROCESS. 
	WAITS TILL THE CHILD PROCESS IS FINISHED EXECUTING.
 */
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

void	shell_process(t_cmds *cmd, int cmd_cnt, int *pipe_fd)
{
	pid_t	pid;
	int		i;

	i = 0;
	pid = -1;
	execute_pipeline(cmd, cmd_cnt, pipe_fd, &pid);
	close_file_descriptors(pipe_fd, cmd_cnt);
	while (1)
		if (wait(NULL) == -1)
			break ;
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
	if (cmd_cnt > 1)
		open_pipes(cmd, cmd_cnt, pipe_fd);
	shell_process(cmd, cmd_cnt, pipe_fd);
	cleanup(cmd_cnt, cmd, pipe_fd);
	return (EXIT_SUCCESS);
}
