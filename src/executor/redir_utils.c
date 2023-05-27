/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/16 15:12:25 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/05/27 10:15:33 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	close_pipes(int *pipe_fd, int cmd_cnt, int current_cmd, int used)
{
	int	i;

	i = 0;
	while (i < 2 * (cmd_cnt - 1))
	{
		if (used == 1)
		{
			if (close(pipe_fd[i]) < 0)
				p_error("close", EXIT_FAILURE);
		}
		else if (used == 2)
		{
			if (i == (2 * (current_cmd - 1)) || i == (2 * current_cmd + 1))
				if (close(pipe_fd[i]) < 0)
					p_error("close", EXIT_FAILURE);
		}
		else
		{
			if (i != (2 * (current_cmd - 1)) && i != (2 * current_cmd + 1))
				if (close(pipe_fd[i]) < 0)
					p_error("close", EXIT_FAILURE);
		}
		i++;
	}
}

void	child_signal_handler(int signum)
{
	if (signum == SIGQUIT)
		exit(3);
	if (signum == SIGINT)
		exit(2);
}
