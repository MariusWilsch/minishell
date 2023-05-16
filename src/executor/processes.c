/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:40:17 by tklouwer          #+#    #+#             */
/*   Updated: 2023/05/16 11:03:49 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"


void	create_process(t_cmds *cmd, int cmd_cnt, int *pipe_fd, pid_t *pid)
{
	int i;

	i = 0;
	while (i < cmd_cnt)
	{
		if (cmd[i].cmd_type == BUILT_IN_EXE
			&& !(ft_strncmp("echo", cmd[i].cmd_path, 4) == 0))
		{
			exec_builtin(cmd->cmd_path, cmd->argc, cmd->argv, cmd->env);
		}
		else
		{
			pid[i] = fork();
			if (pid[i] < 0)
				p_error("fork", 1);
			else if (pid[i] == 0)
			{
				child_process(cmd, i, cmd_cnt, pipe_fd);
				exit(EXIT_SUCCESS);
			}
		}
		i++;
	}
}

/* EXECUTES THE ACTIONS THAT NEEDS TO BE PERFORMED FOR THE PARENT PROCESS. 
	WAITS TILL THE CHILD PROCESS IS FINISHED EXECUTING, CLOSES THE FD'S.
 */
void	parent_process(int *pipe_fd, int i, int curr, pid_t child_pid)
{
	if (curr > 0)
		close(pipe_fd[2 * (curr - 1)]);
	if (curr < i - 1)
		close(pipe_fd[2 * curr + 1]);
	if (waitpid(child_pid, &g_status, 0) == -1)
	{
		if (errno != ECHILD)
			p_error("waitpid", 1);
		else 
		{
			g_status = EXIT_SUCCESS;
			return ;
		}
	}
	g_status = (((g_status) >> 8) & 0xFF);
}

void child_signal_handler(int signum)
{
    if (signum == SIGINT)
    {
        exit(g_status = 130);
    }
}

int	child_process(t_cmds *cmd, int i, int cmd_cnt, int *pipe_fd)
{
	int	heredoc_fd;

	heredoc_fd = -1;
	signal(SIGINT, child_signal_handler);
	handle_heredoc(cmd + i, &heredoc_fd);
	redirect_pipe_fd(i, cmd_cnt, pipe_fd, heredoc_fd);
	close_pipes(pipe_fd, cmd_cnt, i, 0);
	if (heredoc_fd >= 0)
		close(heredoc_fd);
	if (cmd[i].redir)
	{
		if (!heredoc_fd)
			cmd[i].in_fd = pipe_fd[0];
		cmd[i].out_fd = pipe_fd[1];
		redirect_command_fd(&cmd[i]);
	}
	else if (cmd->cmd_type == CMD_EXE)
		execute_command(&cmd[i]);
	else if (cmd->cmd_type == BUILT_IN_EXE)
		exec_builtin(cmd->cmd_path, cmd->argc, cmd->argv, cmd->env);
	exit(EXIT_SUCCESS);
}

void	shell_process(t_cmds *cmd, int cmd_cnt, int *pipe_fd)
{
	pid_t	*pid;
	int		i;

	i = 0;
	pid = malloc(sizeof(pid_t) * cmd_cnt);
	if (pid == NULL) 
	{
		p_error("malloc", EXIT_FAILURE);
	}
	create_process(cmd, cmd_cnt, pipe_fd, pid);
	while (i < cmd_cnt)
	{
		parent_process(pipe_fd, cmd_cnt, i, pid[i]);
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
	int				i;
	int				*pipe_fd;

	i = 0;
	if (ft_strcmp("exit", head->arg) == 0)
	{
		rl_clear_history();
		exit(0);
	}
	cmd = create_structs(head, &cmd_cnt, env_l);
	pipe_fd = (int *)ft_calloc(2 * (cmd_cnt), sizeof(int));
	while (i < cmd_cnt)
	{
		if (pipe(pipe_fd + 2 * i) < 0)
			p_error("pipe", EXIT_FAILURE);
		i++;
	}
	shell_process(cmd, cmd_cnt, pipe_fd);
	cleanup(cmd_cnt, cmd);
	free(pipe_fd);
	return (EXIT_SUCCESS);
}
