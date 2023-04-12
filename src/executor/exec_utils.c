/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 16:21:11 by tklouwer          #+#    #+#             */
/*   Updated: 2023/04/12 14:00:29 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_command(t_cmds *cmd)
{
	if (cmd->argv)
    	free(cmd->argv);
    if (cmd->redir)
			free(cmd->redir);
    free(cmd);
}

int	wr_dup2(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
	perror("Dup2");
	exit (EXIT_FAILURE);
}
int		p_error(char *str, int status)
{
	perror(str);
	sh_exit(status);
}
void	execute_command(t_cmds *head)
{
	if (head->cmd_type == CMD_EXE)
	{
		if (execve(head->cmd_path, head->argv, NULL) == -1)
			p_error("Execve failed", 127);
	}
	else
		exec_builtin(head->cmd_path, head->argc, head->argv);
	free_command(head);
}
