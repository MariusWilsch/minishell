/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:40:17 by tklouwer          #+#    #+#             */
/*   Updated: 2023/04/12 14:07:26 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>

/* 
	Parse the command line input to identify the redirects and the corresponding input and output files.
	Handle any here-documents (<<) by creating temporary files and writing the content to them.
	Use fork() to create a child process for each command or pipe stage.
	In each child process, perform the necessary input and output redirections using dup2():
	If the command reads from a file or here-document, redirect stdin (file descriptor 0) to the appropriate file using dup2().
	If the command writes to a file, redirect stdout (file descriptor 1) to the appropriate file using dup2().
	If the command is part of a pipeline, redirect stdin and/or stdout to the corresponding read and write ends of the pipes using dup2().
	Close any unused file descriptors in the child process.
	Execute the command in the child process using an exec() family function (e.g., execl(), execv(), execvp(), etc.).
	In the parent process, wait for the child processes to complete using wait() or waitpid().
 */


int close_pipes(int *pipe_fd, int pipes)
{
	int i;

	i = 0;
	while (i < pipes)
	{
		if (pipe_fd[i] >= 0)
			close(pipe_fd[i]);
		i++;
	}
}

// Change the name of int i to int cmd_count
// int executor(t_args *head)
// {
// 	int			cmd_cnt;
// 	pid_t		child;
// 	t_cmds	*cmd;
	
// 	cmd_cnt = 0;
// 	cmd = create_structs(head, &cmd_cnt);
// 	if (cmd_cnt == 1 && !cmd[0].redir)
// 	{
// 			execute_command(cmd);
// 			return (EXIT_SUCCESS);
// 	}
// 	// while (cmd_cnt < 1)
// 	// {
// 	// 		int pipe_fd[2 * cmd_cnt];
// 	// 		if (pipe(pipe_fd + 2 * cmd_cnt) == -1)
// 	// 		{
// 	// 				perror("pipe");
// 	// 				return (EXIT_FAILURE);
// 	// 		}
// 	// 		child = fork();
// 	// 		if (child < 0)
// 	// 				perror("fork");
// 	// 		if (child == 0)
// 	// 				child_process(cmd, pipe_fd);
// 	// 		cmd_cnt++;
// 	// }
// 	return (EXIT_SUCCESS);
// }

int executor(t_args *head)
{
	int			cmd_cnt;
	t_cmds *cmd = create_structs(head, &cmd_cnt);
	int			pipe_fd[2 * cmd_cnt];
	int			i;

	// ft_printf("cmd_cnt: %d\n", cmd_cnt *	2);
	i = 0;
	while (i < cmd_cnt)
	{
		if (pipe(pipe_fd + 2 * cmd_cnt) == -1)
			perror("pipe");
		pid_t pid = fork();
		if (pid < 0)
			p_error("fork", 1);
		if (pid == 0)
		{
			// if (cmd[i].redir != NULL)
			// 	handle_redirects(&cmd[i], pipe_fd);
			// else
				execute_command(&cmd[i]);
		}
		else
		{
			int	status;
			close_pipes(pipe_fd, 2 * cmd_cnt);
			if (waitpid(pid, &status, 0) == -1)
				p_error("waitpid", 1);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

	// for (i = 0; i < 2; i++) 
	// {
	// 		int flags = fcntl(pipe_fd[i], F_GETFD);
	// 		if (flags == -1) 
	// 		{
	// 				perror("fcntl");
	// 				exit(EXIT_FAILURE);
	// 		}
	// 		if (flags != 0)
	// 				ft_printf("File descriptor %d is still open\n", pipe_fd[i]);
	// 		else
	// 			ft_printf("File descriptor %d is closed", pipe_fd[i]);
	// 	}