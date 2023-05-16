/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_io.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: verdant <verdant@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/29 16:26:30 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/05/16 10:35:09 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"


void	redirect_pipe_fd(int i, int cmd_cnt, int *pipe_fd, int heredoc_fd)
{
	if (i > 0 && heredoc_fd < 0)
	{
		if (dup2(pipe_fd[2 * (i - 1)], STDIN_FILENO) < 0)
			perror("dup2");
	}
	if (i < cmd_cnt - 1)
	{
		if (dup2(pipe_fd[2 * i + 1], STDOUT_FILENO) < 0)
			perror("dup2");
	}
}
int	redirect_command_fd(t_cmds *head)
{
	int	i;

	i = 0;
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
