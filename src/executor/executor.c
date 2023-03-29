/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/22 16:40:17 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/03/29 15:18:54 by tklouwer      ########   odam.nl         */
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
void	execute_command(t_cmds *head)
{
	if (!head->cmd_path)
	{
		perror("Command not found");
		sh_exit(127);
	}
	if (head->cmd_type == CMD_EXE) 
	{
		if (execve(head->cmd_path, head->argv, NULL) == -1)
		{
			perror("Execve failed");
			sh_exit(127);		
		}
	}
	else
		exec_builtin(head->cmd_path, 1, head->argv);
}
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

int wr_perror(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}
int redirect_io(t_cmds *head, int *end)
{
    int fd = -1;
    int open_flags;

    if (head->redir->type == TRUNC || head->redir->type == APPEND)
    {
        open_flags = O_WRONLY | O_CREAT;
        if (head->redir->type == TRUNC) open_flags |= O_TRUNC;
        else open_flags |= O_APPEND;

        fd = open(head->redir->filename, open_flags, 0644);
        dup2(fd, STDOUT_FILENO);
    }
    else if (head->redir->type == INPUT)
    {
        fd = open(head->redir->filename, O_RDONLY);
        dup2(fd, STDIN_FILENO);
    }
    else if (head->redir->type == INPUT_EOF)
    {
        read_heredoc(*head->argv);
    }
    if (fd == -1)
        wr_perror("open");
    close(fd);
    return EXIT_SUCCESS;
}
int child_process(t_cmds *head, int *end)
{
	redirect_io(head, end);	
	execute_command(head);
}
int executor(t_args *head)
{
	int i;
	pid_t child;
	t_cmds *cmd;

	i = 0;
	cmd = create_structs(head);
	if (head->cmnd_count == 1)
	{
		execute_command(cmd);
		return (EXIT_SUCCESS);
	}
	while (i < 1)
	{
		int pipe_fd[2 * head->cmnd_count];
		if (pipe(pipe_fd + 2 * i) == -1)
		{
			perror("pipe");
			return (EXIT_FAILURE);
		}
		child = fork();
		if (child < 0)
			wr_perror("fork");
		if (child == 0)
			child_process(head, pipe_fd);
		i++;
	}
	return (EXIT_SUCCESS);
}
