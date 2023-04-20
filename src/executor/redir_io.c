/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_io.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: verdant <verdant@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/29 16:26:30 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/04/20 14:18:12 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

int handle_redirects(t_cmds *head)
{
	if (head->redir->type == TRUNC || head->redir->type == APPEND)
	{
		redirect_output(head);
	}
	else
	{
		redirect_input(head);
	}
	execute_command(head);
	return (EXIT_SUCCESS);
}
int heredoc(const char *delimiter)
{
    int pipefd[2];
    char *line = NULL;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }
    while ((line = readline("")) != NULL)
    {
        if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
        {
            free(line);
            break;
        }
        write(pipefd[1], line, ft_strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    close(pipefd[1]);
    return pipefd[0];
}
int redirect_input(t_cmds *head)
{
	int fd = -1;

	if (head->redir->type == INPUT)
	{
			fd = open(head->redir->filename, O_RDONLY);
	}
	else if (head->redir->type == INPUT_EOF)
	{
		printf("%s", head->redir->filename);
		fd = heredoc(head->redir->filename);
	}
	if (fd == -1)
			perror("open");
	if (dup2(fd, STDIN_FILENO) < 0)
		wr_dup2(fd, STDIN_FILENO);
	close(fd);
	return EXIT_SUCCESS;
}
int redirect_output(t_cmds *head)
{
    int fd = -1;

    if (head->redir->type == TRUNC)
        fd = open(head->redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (head->redir->type == APPEND)
        fd = open(head->redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
    if (dup2(fd, STDOUT_FILENO) < 0)
        wr_dup2(fd, STDOUT_FILENO);
    close(fd);
    return (EXIT_SUCCESS);
}
