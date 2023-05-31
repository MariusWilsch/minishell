/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_io.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: verdant <verdant@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/29 16:26:30 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/05/31 11:29:33 by dickklouwer   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void redirect_pipe_fd(int i, int cmd_cnt, int *pipe_fd)
{
    // If not the first command, redirect input from the previous command
    if (i > 0)
    {
        if (dup2(pipe_fd[2 * (i - 1)], STDIN_FILENO) == -1)
        {
            p_error("dup2", EXIT_FAILURE);
        }
    }

    // If not the last command, redirect output to the next command
    if (i < cmd_cnt - 1)
    {
        if (dup2(pipe_fd[2 * i + 1], STDOUT_FILENO) == -1)
        {
            p_error("dup2", EXIT_FAILURE);
        }
    }

    // Close all pipe file descriptors
    for (int j = 0; j < 2 * cmd_cnt; j++)
    {
        if (close(pipe_fd[j]) == -1)
        {
            p_error("close", EXIT_FAILURE);
        }
    }
}


int	redirect_command_fd(t_cmds *head)
{
	int	i;

	i = 0;
	while (i < head->redircnt)
	{
		if (head->redir[i].type == TRUNC || head->redir[i].type == APPEND)
			redirect_output(&head->redir[i]);
		else
			redirect_input(&head->redir[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	redirect_input(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir == NULL)
		p_error("NULL redir", EXIT_FAILURE);
	if (redir->type == INPUT)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == INPUT_EOF)
		return (EXIT_SUCCESS);
	if (fd < 0)
		p_error("open", EXIT_FAILURE);
	if (dup2(fd, STDIN_FILENO) < 0)
		wr_dup2(fd, STDIN_FILENO);
	if (close(fd) < 0)
		p_error("close", EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	redirect_output(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir == NULL)
		p_error("NULL redir", EXIT_FAILURE);
	if (redir->type == TRUNC)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		p_error("open", EXIT_FAILURE);
	if (dup2(fd, STDOUT_FILENO) < 0)
		wr_dup2(fd, STDOUT_FILENO);
	if (close(fd) < 0)
		p_error("close", EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
