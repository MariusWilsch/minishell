/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 11:27:54 by mwilsch           #+#    #+#             */
/*   Updated: 2023/05/18 15:05:20 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void print_structs(t_cmds *cmd, int cmd_cnt)
{
	int i;

	i = 0;
	while (i < cmd_cnt)
	{
		printf("cmd_path: %s\n", cmd[i].cmd_path);
		printf("argc: %d\n", cmd[i].argc);
		printf("redircnt: %d\n", cmd[i].redircnt);
		// printf("in_fd: %d\n", cmd[i].in_fd);
		// printf("out_fd: %d\n", cmd[i].out_fd);
		// printf("status: %d\n", cmd[i].status);
		// printf("cmd_type: %d\n", cmd[i].cmd_type);
		i++;
	}
}

//Write a funtion which will print t_args *head list

void	init_members(t_cmds *cmd)
{
	cmd->cmd_path = NULL;
	cmd->argv = NULL;
	cmd->argc = 1;
	cmd->redircnt = 0;
	cmd->in_fd = 0;
	cmd->out_fd = 0;
	cmd->status = 0;
	// cmd->env = env;
	// cmd->redir.filename = NULL;
	// cmd->redir.redirc = 0;
	// cmd->redir.type = 0;
	// cmd->cmd_type = 0;
}

void	arg_counter(t_args *head, t_cmds *cmd, int cmd_cnt)
{
	t_args *node;

	node = head;
	init_members(cmd);
	while (node && node->type != PIPE)
	{
		if (node->type == CMD && node->err_tok == OK)
			cmd->cmd_path = node->arg;
		if (node->type == ARG)
			cmd->argc++;
		if (node->type == REDIR)
			cmd->redircnt++;
		node = node->next;
	}
}




t_cmds *create_structs(t_args *head, int *cmd_cnt, t_env **env)
{
	t_args	*tmp;
	t_cmds	*cmds;
	int			i;

	tmp = head;
	i = 0;
	while (tmp)
	{
		if (tmp->type == PIPE)
			(*cmd_cnt)++;
		tmp = tmp->next;
	}
	cmds = (t_cmds *)malloc(sizeof(t_cmds) * (*cmd_cnt));
	if (!cmds)
		return (NULL);
	while (i < (*cmd_cnt))
	{
		arg_counter(head, &cmds[i], i);
		i++;
	}
	print_structs(cmds, *cmd_cnt);
	return (cmds);
}


