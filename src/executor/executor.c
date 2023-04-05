/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:40:17 by tklouwer          #+#    #+#             */
/*   Updated: 2023/04/03 11:41:46 by verdant          ###   ########.fr       */
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


char *read_heredoc(const char *delimiter) // CHECK REWRITE
{
		char *heredoc;
		char *line;
		size_t line_len = 0;
		ssize_t nread;

		while ((nread = getline(&line, &line_len, stdin)) != -1) {
				if (strncmp(line, delimiter, strlen(delimiter)) == 0 && line[strlen(delimiter)] == '\n') 
						break;
				if (heredoc == NULL)
						heredoc = strdup(line);
				else {
						heredoc = realloc(heredoc, strlen(heredoc) + nread + 1);
						strcat(heredoc, line);
				}
		}
		free(line);
		return heredoc;
}

int child_process(t_cmds *head, int *end)
{
	if (head->redir->type == TRUNC || head->redir->type == APPEND)
		redirect_output(head, end);
	else
		redirect_input(head, end);
	execute_command(head);
}

// Change the name of int i to int cmd_count
int executor(t_args *head)
{
	int			cmd_cnt;
	pid_t		child;
	t_cmds	*cmd;
	
	cmd_cnt = 0;
	cmd = create_structs(head, &cmd_cnt);
	if (cmd_cnt == 1 && !cmd[0].redir)
	{
			execute_command(cmd);
			return (EXIT_SUCCESS);
	}
	// while (cmd_cnt < 1)
	// {
	// 		int pipe_fd[2 * cmd_cnt];
	// 		if (pipe(pipe_fd + 2 * cmd_cnt) == -1)
	// 		{
	// 				perror("pipe");
	// 				return (EXIT_FAILURE);
	// 		}
	// 		child = fork();
	// 		if (child < 0)
	// 				perror("fork");
	// 		if (child == 0)
	// 				child_process(cmd, pipe_fd);
	// 		cmd_cnt++;
	// }
	return (EXIT_SUCCESS);
}
