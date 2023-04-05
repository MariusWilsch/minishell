/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_io.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 16:26:30 by tklouwer          #+#    #+#             */
/*   Updated: 2023/04/03 11:04:54 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int redirect_input(t_cmds *head, int *end)
{
    int fd = -1;

    if (head->redir->type == INPUT)
    {
        fd = open(head->redir->filename, O_RDONLY);
        if (dup2(fd, STDIN_FILENO) < 0)
			wr_dup2(fd, end[1]);
		if (dup2(end[1], STDOUT_FILENO) < 0)
			wr_dup2(fd, end[1]);
    }
    // else if (head->redir->type == INPUT_EOF)
    // {
    //     read_heredoc(*head->argv);
    // }
    if (fd == -1)
        perror("open");
    close(fd);
	close(end[0]);
    return EXIT_SUCCESS;
}
int	redirect_output(t_cmds *head, int *end)
{
	int fd;

	if (head->redir->type == TRUNC)
    {
        fd = open(head->redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (dup2(fd, STDOUT_FILENO) < 0)
			wr_dup2(fd, end[0]);
    }
	if (head->redir->type == APPEND)
	{
		fd = open(head->redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (dup2(fd, STDOUT_FILENO) < 0)
			wr_dup2(fd, end[0]);
	}
	if (dup2(end[0], STDIN_FILENO) < 0)
		wr_dup2(fd, end[0]);
	close(fd);
	close(end[1]);
	return (EXIT_SUCCESS);
}