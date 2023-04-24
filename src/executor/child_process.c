/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   child_process.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/24 11:45:38 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/04/24 13:53:13 by tklouwer      ########   odam.nl         */
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

int	heredoc(const char *delimiter)
{
	int		pipefd[2];
	char	*line;

	line = NULL;
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("");
		if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

void	handle_heredoc(t_cmds *cmd, int *heredoc_fd)
{
	if (cmd->redir && cmd->redir->type == INPUT_EOF)
	{
		*heredoc_fd = heredoc(cmd->redir->filename);
		if (*heredoc_fd >= 0)
		{
			if (dup2(*heredoc_fd, STDIN_FILENO) < 0)
				perror("dup2 heredoc");
		}
	}
}

void	handle_redirections(int i, int cmd_cnt, int *pipe_fd, int heredoc_fd)
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

int	child_process(t_cmds *cmd, int i, int cmd_cnt, int *pipe_fd)
{
	int	heredoc_fd;

	heredoc_fd = -1;
	handle_heredoc(cmd + i, &heredoc_fd);
	handle_redirections(i, cmd_cnt, pipe_fd, heredoc_fd);
	close_pipes(pipe_fd, cmd_cnt, i, 0);
	if (heredoc_fd >= 0)
		close(heredoc_fd);
	if (cmd[i].redir)
	{
		if (!heredoc_fd)
			cmd[i].in_fd = pipe_fd[0];
		cmd[i].out_fd = pipe_fd[1];
		handle_redirects(&cmd[i]);
	}
	else
	{
		execute_command(&cmd[i]);
	}
	exit(EXIT_SUCCESS);
}
