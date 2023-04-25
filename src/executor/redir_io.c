/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_io.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: verdant <verdant@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/29 16:26:30 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/04/25 14:45:02 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <readline/readline.h>
#include <readline/history.h>

int	handle_redirects(t_cmds *head)
{
	int i = 0;
	while (i < head->redir->redirc)
	{
		if (head->redir[i].type == TRUNC || head->redir[i].type == APPEND)
		{
			redirect_output(&head->redir[i]);
		}
		else
		{
			redirect_input(&head->redir[i]);
		}
		i++;
	}
	execute_command(head);
	return (EXIT_SUCCESS);
}

int	redirect_input(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == INPUT)
	{
		fd = open(redir->filename, O_RDONLY);
	}
	else if (redir->type == INPUT_EOF)
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

int	redirect_output(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == TRUNC)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
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
