/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_old.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 17:41:28 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/24 11:37:29 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_args	*fill_struct(t_cmds *cmd, t_args	*head, int argc, int operc)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	cmd->name = head->arg;
	cmd->args = malloc(sizeof(char *) * (argc + 1));
	cmd->oper = malloc(sizeof(char *) * (operc + 1));
	if (!cmd->args || !cmd->oper)
		return (NULL);
	cmd->args[argc] = NULL;
	cmd->oper[operc] = NULL;
	if (argc == 0 && operc == 0)
		return (head->next);
	while (head != NULL && (head->type != CMD || head->type != BUILT_IN))
	{
		if (i < argc && (head->type == ARG || head->type == QUOTE_ARG))
			cmd->args[i++] = head->arg;
		if (k < operc && head->type == REDIRECT)
			cmd->oper[k++] = head->arg;
		head = head->next;
		if (i == argc && k == operc)
			break;
	}
	return (head);
}

t_cmds	*cnt_rest(t_args *node, t_cmds *cmds, t_args *head, int cmd_cnt)
{
	int	i;
	int	arg_cnt;
	int	operator_cnt;
	
	i = 0;
	if (node->type == CMD || node->type == BUILT_IN)
		node = node->next;
	while (i < cmd_cnt)
	{
		arg_cnt = 0;
		operator_cnt = 0;
		while (node != NULL && node->type != CMD && node->type != BUILT_IN)
		{
			if ((node->type == ARG || node->type == QUOTE_ARG))
				arg_cnt++;
			if (node->type == REDIRECT)
				operator_cnt++;
			node = node->next;
		}
		head = fill_struct(&cmds[i++], head, arg_cnt, operator_cnt);
		if (node != NULL)
			node = node->next;
	}
	return (cmds);
}
		// ft_printf("CMD_NUM: %d\tARG_CNT: %d\tOPER: %d\n", i, arg_cnt, operator_cnt);

		// if (arg_cnt != 0)
		// {
		// 	cmds[i]->args = malloc(sizeof(char *) * ())
		// }


void	print_struct(t_cmds *cmds, int cmd_limit)
{
	int i = 0;
	int k = 0;
	int	cmd_cnt = 0;

	while (cmd_cnt < cmd_limit)
	{
		ft_printf("cmd name: %s\n", cmds[cmd_cnt].cmd);
		for (i = 0; cmds[cmd_cnt].args[i] != NULL; i++)
			ft_printf("args: %s\n", cmds[cmd_cnt].args[i]);
		for (k = 0; cmds[cmd_cnt].oper[k] != NULL; k++)
			ft_printf("oper_args: %s\n", cmds[cmd_cnt].oper[k]);
		ft_printf("\n");
		cmd_cnt++;
	}
}


/**
 * 
 * @note 1. Quote ARG are not working yet
 * @note 2. Err Toks for CMD and ARGS do not matter but for oper they do. 
 * @note 2.1 I either change the env_var_err thing to do in the executor or add the err tok to the struct
 * @note 2.2 If I add it to the struct it would be best if change the char **oper to a small linked list
*/
bool	executor(t_args *head)
{
	t_args	*node;
	t_cmds	*cmds;
	int cmd_cnt;

	node = head;
	cmd_cnt = 0;
	while (node != NULL)
	{
		if (node->type == CMD || node->type == BUILT_IN)
			cmd_cnt++;
		node = node->next;
	}
	cmds = malloc(sizeof(t_cmds) * cmd_cnt);
	if (!cmds)
		return (false);
	node = head;
	cmds = cnt_rest(node, cmds, head, cmd_cnt);
	print_struct(cmds, cmd_cnt);
	return (true);
}
