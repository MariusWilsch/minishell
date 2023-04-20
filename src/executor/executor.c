/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: verdant <verdant@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/22 16:40:17 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/04/20 13:32:00 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>


void close_pipes(int *pipe_fd, int cmd_cnt, int current_cmd, int used)
{
	int i;

	i = 0;
	while (i < 2 * (cmd_cnt - 1))
    {
        if (used)
        {
            if (i == 2 * (current_cmd - 1) || i == 2 * current_cmd + 1)
                close(pipe_fd[i]);
        }
        else
        {
            if (i != 2 * (current_cmd - 1) && i != 2 * current_cmd + 1)
                close(pipe_fd[i]);
        }
		i++;
    }
}
/* EXECUTES THE ACTIONS THAT NEEDS TO BE PERFORMED FOR THE PARENT PROCESS. 
	WAITS TILL THE CHILD PROCESS IS FINISHED EXECUTING, CLOSES THE FD'S.
 */
void parent_process(int *pipe_fd, int cmd_cnt, int current_cmd, pid_t child_pid)
{
    if (current_cmd > 0)
        close(pipe_fd[2 * (current_cmd - 1)]);
    if (current_cmd < cmd_cnt - 1)
        close(pipe_fd[2 * current_cmd + 1]);

    int status;
    if (waitpid(child_pid, &status, 0) == -1)
        p_error("waitpid", 1);
}

/* EXECUTES THE ACTIONS THAT NEEDS TO BE PERFORMED FOR THE CHILD PROCESS. 
	SETS UP THE IN- AND OUTPUT REDIRECTION BASED ON CMDS POSITION.
 */
int child_process(t_cmds *cmd, int i, int cmd_cnt, int *pipe_fd)
{
	if (i > 0)
	{
		if (dup2(pipe_fd[2 * (i - 1)], STDIN_FILENO) < 0)
			perror("dup2");
	}
	if (i < cmd_cnt - 1)
	{
		if (dup2(pipe_fd[2 * i + 1], STDOUT_FILENO) < 0)
			perror("dup2");
	}
	close_pipes(pipe_fd, cmd_cnt, i, 0);
	if (cmd[i].redir) // IF IT HAS REDIRECTION, SETS UP THE REDIRECTS. 
	{
		cmd[i].in_fd = pipe_fd[0];
		cmd[i].out_fd = pipe_fd[1];
		handle_redirects(&cmd[i]);
	}
	else
		execute_command(&cmd[i]);
	exit(EXIT_SUCCESS);
}

void shell_process(t_cmds *cmd, int cmd_cnt)
{
	if (cmd_cnt > 1)
		return ;
	else
		exec_builtin(cmd->cmd_path, cmd->argc, cmd->argv);
}
/* RESPONSIBLE FOR SETTING UP THE NECESSARY STRUCTURES FOR HANDLING COMMANDS
	AND MANAGING THE CHILD PROCESSES.
	- CMND_CNT = NUMBER OF COMMANDS.
	- PIPE_FD = ARRAY OF FILE DESCRIPTORS FOR THE PIPES
	- BUILT-INS NEED TO BE EXECUTED IN THE PARENT PROCESS. SO WE DONT FORK THEM. 
 */
int executor(t_args *head)
{
	t_cmds 		*cmd;
	int			cmd_cnt;
	int			i;

	i = 0;
	cmd = create_structs(head, &cmd_cnt);
	int		pipe_fd[2 * (cmd_cnt - 1)];
	int j = 0;
	while(j < cmd_cnt)
	{
		if (pipe(pipe_fd + 2 * j) < 0) // SETS UP PIPES
			perror("pipe");
		j++;
	}
	while (i < cmd_cnt)
	{
		if (cmd[i].cmd_type == BUILT_IN_EXE && !(ft_strncmp("echo", cmd[i].cmd_path, 4) == 0)) // BUILT-IN SO NO FORK
			shell_process(cmd, cmd_cnt);
		else {
			pid_t pid = fork(); // CREATES CHILD PROCESS WHERE IT EXECUTES EACH INDIVIDUAL COMMAND
			if (pid < 0)
				p_error("fork", 1);
			else if (pid == 0)
			{
				child_process(cmd, i, cmd_cnt, pipe_fd);
			}
			else
			{
				parent_process(pipe_fd, cmd_cnt, i, pid);
			}
		}
		i++;
	}
	return (EXIT_SUCCESS);
}