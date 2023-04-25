/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:40:17 by tklouwer          #+#    #+#             */
/*   Updated: 2023/04/25 10:17:32 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include <string.h>

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
		p_error("waitpid", 1);
}

int	shell_process(t_cmds *cmd, int cmd_cnt, int *pipe_fd)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (i < cmd_cnt)
	{
		if (cmd[i].cmd_type == BUILT_IN_EXE && !(ft_strncmp("echo", cmd[i].cmd_path, 4) == 0))
			exec_builtin(cmd->cmd_path, cmd->argc, cmd->argv, cmd->env);
		else
		{
			pid = fork();
			if (pid < 0)
				p_error("fork", 1);
			else if (pid == 0)
				child_process(cmd, i, cmd_cnt, pipe_fd);
			else
				parent_process(pipe_fd, cmd_cnt, i, pid);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}


void free_env_list(t_env *env_list) {
    t_env *current = env_list;
    t_env *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        if (temp->key != NULL) {
            free(temp->key);
        }
        if (temp->value != NULL) {
            free(temp->value);
        }
        free(temp);
    }
}


void	leaks(void)
{
	system("leaks -q minishell");
}

/* RESPONSIBLE FOR SETTING UP THE NECESSARY STRUCTURES FOR HANDLING COMMANDS
	AND MANAGING THE CHILD PROCESSES.
	- CMND_CNT = NUMBER OF COMMANDS.
	- PIPE_FD = ARRAY OF FILE DESCRIPTORS FOR THE PIPES
	- BUILT-INS NEED TO BE EXECUTED IN THE PARENT PROCESS. SO WE DONT FORK THEM. 
 */
int	executor(t_args *head, t_env **env_l)
{
	t_cmds		*cmd;
	int			cmd_cnt;
	int			i;
	int			*pipe_fd;

	if (ft_strcmp("exit", head->arg) == 0)
	{
		atexit(leaks);
		exit(0);
	}
	i = 0;
	cmd = create_structs(head, &cmd_cnt, env_l);
	pipe_fd = (int *)ft_calloc(2 * (cmd_cnt), sizeof(int));
	while (i < cmd_cnt)
	{
		if (pipe(pipe_fd + 2 * i) < 0)
			perror("pipe");
		i++;
	}
	shell_process(cmd, cmd_cnt, pipe_fd);
	while (cmd_cnt--)
	{
		free(pipe_fd);
		free(cmd[cmd_cnt].argv);
		free(cmd);
	}
	return (EXIT_SUCCESS);
}
