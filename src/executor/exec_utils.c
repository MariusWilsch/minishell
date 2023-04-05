/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 16:21:11 by tklouwer          #+#    #+#             */
/*   Updated: 2023/04/04 12:18:31 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
void	execute_command(t_cmds *cmd)
{
	if (!cmd->cmd_path)
		p_error("Command not found", 127);
	if (cmd->cmd_type == CMD_EXE) 
	{
		if (execve(cmd->cmd_path, cmd->argv, NULL) == -1)
			p_error("Execve failed", 127);
	}
	else if (cmd->cmd_type == BUILT_IN_EXE)
		exec_builtin(cmd->cmd_path, cmd->argc, cmd->argv);
}
