/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/22 16:40:17 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/03/24 16:04:31 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/executor.h"


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

int redirect(char *cmd)
{
    if (ft_strnstr(cmd, ">>", 2))
        return (1);
    if (ft_strnstr(cmd, ">", 1))
        return (2);
    if (ft_strnstr(cmd, "<<", 2))
        return (3);
    if (ft_strnstr(cmd, "<", 1))
        return (4);
}
void	execute_command(t_cmds *head)
{
	if (!head->cmd_path)
	{
		perror("Command not found");
	}
	if (execve(head->cmd_path, head->args, NULL) == -1)
	{
		perror("Execve failed");
	}
}
int executor(t_args *head)
{

}

int executor2()
{
        int     num_commands;
    int     i;
    int     pipe_fd[2 * num_commands];
    int     redirect_case;
    pid_t   child_prcs;

    i = 0;
    while (i < num_commands)
    {
        if (pipe(pipe_fd + 2 * i) == -1)
        {
            perror("pipe");
            return (EXIT_FAILURE);
        }
    }
    i = 0;
    while (i < num_commands)
    {
        child_prcs = fork();
        if (child_prcs < 0)
            perror("Fork:");
        else if (child_prcs == 0)
            child_process(head);
    }
}
int child_process(t_cmds *head)
{
    int     redirect_case;
    int     outfile;

    redirect_case = redirect(head->cmd_path);
    if (redirect_case == 1 || redirect_case == 2)
    {
        if (redirect_case == 1)
            outfile = open(head->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            outfile = open(head->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outfile == -1) 
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(outfile, STDOUT_FILENO);
        close(outfile);
    }
    return (EXIT_SUCCESS);
}
