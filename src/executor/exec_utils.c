/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 16:21:11 by tklouwer          #+#    #+#             */
/*   Updated: 2023/05/12 12:47:34 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	wr_dup2(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
	perror("Dup2");
	exit (EXIT_FAILURE);
}

int	p_error(char *str, int status)
{
	perror(str);
	exit(status);
	return (EXIT_FAILURE);
}

void	execute_command(t_cmds *cmd)
{
	t_env	*found;

	if (cmd->cmd_type == CMD_EXE)
	{
		if (exisit_env(cmd->env, "PATH", &found) == -1)
		{
			ft_printf("minishell: %s: No such file or dir\n", cmd->cmd_path);
			exit(127);
		}
		if (ft_strcmp(cmd->cmd_path, "./minishell") == 0)
		{
			if (execve(cmd->cmd_path, cmd->argv, convert_data(*cmd->env)) == -1)
				p_error("Execve failed", 127);
		}
		else if (execve(cmd->cmd_path, cmd->argv, NULL) == -1)
			p_error("Execve failed", 127);
	}
	else
		exec_builtin(cmd->cmd_path, cmd->argc, cmd->argv, cmd->env);
}

static void	cleanup_redir(t_redir *redir, int redir_c)
{
	int	i;

	i = 0;
	while (redir_c-- > 0)
	{
		free(redir[redir_c].filename);
		free(redir[redir_c].redirect);
	}
	free(redir);
}

void	cleanup(int cmd_cnt, t_cmds *cmd)
{
	while (cmd_cnt--)
	{
		free(cmd[cmd_cnt].argv);
		if (cmd[cmd_cnt].redir)
			cleanup_redir(cmd[cmd_cnt].redir, cmd[cmd_cnt].redir->redirc);
	}
	free(cmd);
}
