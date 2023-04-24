/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_io.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: verdant <verdant@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/29 16:26:30 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/04/24 13:39:26 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <readline/readline.h>
#include <readline/history.h>

int	handle_redirects(t_cmds *head)
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

int	redirect_input(t_cmds *head)
{
	int	fd;

	fd = -1;
	if (head->redir->type == INPUT)
	{
		fd = open(head->redir->filename, O_RDONLY);
	}
	else if (head->redir->type == INPUT_EOF)
	{
		return (EXIT_SUCCESS);
	}
	if (fd == -1)
		perror("open");
	if (dup2(fd, STDIN_FILENO) < 0)
		wr_dup2(fd, STDIN_FILENO);
	close(fd);
	return (EXIT_SUCCESS);
}

int	redirect_output(t_cmds *head)
{
	int	fd;

	fd = -1;
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
