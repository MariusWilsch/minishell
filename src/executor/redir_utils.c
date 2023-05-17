/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/16 15:12:25 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/05/17 11:25:12 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	close_pipes(int *pipe_fd, int cmd_cnt, int current_cmd, int used)
{
	int	i;

	i = 0;
	while (i < 2 * (cmd_cnt - 1))
	{
		if (used)
		{
			if (i == 2 * (current_cmd - 1) || i == 2 * current_cmd + 1)
				close(pipe_fd[i]);
		}
		else
		{
			if (i != 2 * (current_cmd - 1) && i != 2 * current_cmd + 1)
				close(pipe_fd[i]);
		}
		i++;
	}
}

void	child_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		exit(130);
	}
}
