/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_data_structure.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 11:27:54 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/24 13:16:14 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

bool fill_operation_struct(t_operation *oper, char *str, t_err_tok err_type)
{
	int cnt;
	const int file_length = ft_strlen(str) - cnt;
	const char c = str[0];
	
	cnt = cnt_occur(str, str[0]);
	oper->redirect = NULL;
	oper->filename = NULL;
	if (err_type == ENV_REDIRECT_ERR)
		return (oper->type = ERR, true);
	// if (str[cnt] == ' ')
	// 	cnt += cnt_occur(str + cnt, ' ');
	oper->redirect = ft_substr(str, 0, cnt);
	oper->filename = ft_substr(str, cnt, file_length);
	if (!oper->redirect || !oper->filename)
		return (false);
	if (c == '>' && cnt == 1)
		oper->type = TRUNC;
	if (c == '>' && cnt == 2)
		oper->type = APPEND;
	if (c == '<' && cnt == 1)
		oper->type = INPUT;
	if (c == '<' && cnt == 2)
		oper->type = INPUT_EOF;
	return (true);
}

t_args	*fill_struct(t_cmds *cmd, t_args	*head, int argc, int operc)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	cmd->cmd = head->arg;
	cmd->args = malloc(sizeof(char *) * (argc + 1));
	cmd->oper = malloc(sizeof(t_operation) * operc);
	if (!cmd->args || !cmd->oper)
		return (NULL);
	cmd->args[argc] = NULL;
	if (argc == 0 && operc == 0)
		return (head->next);
	while (head != NULL && (head->type != CMD || head->type != BUILT_IN))
	{
		if (i < argc && (head->type == ARG || head->type == QUOTE_ARG))
			cmd->args[i++] = head->arg;
		if (k < operc && head->type == REDIRECT)
			fill_operation_struct(&cmd->oper[k++], head->arg, head->err_tok);
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
		for (k = 0; k < 1; k++)
		{
			ft_printf("red: %s\t", cmds[cmd_cnt].oper[k].redirect);
			ft_printf("filename: %s\t", cmds[cmd_cnt].oper[k].filename);
			ft_printf("type: %d\n", cmds[cmd_cnt].oper[k].type);
		}
		ft_printf("\n");
		cmd_cnt++;
	}
}


/**
 * 
 * @note What if there there is no cmd but a redirect as the first string
*/
bool	executor(t_args *head)
{
	t_args	*node;
	t_cmds	*cmds;
	int cmd_cnt;

	node = head;
	cmd_cnt = 0;
	if (head->type == REDIRECT)
		cmd_cnt++;
	while (node != NULL)
	{		
		if (node->type == CMD || node->type == BUILT_IN)
			cmd_cnt++;
		node = node->next;
	}
	cmds = malloc(sizeof(t_cmds) * cmd_cnt);
	if (!cmds)
		return (false);
	cmds->cmd = NULL;
	cmds->oper = NULL;
	node = head;
	cmds = cnt_rest(node, cmds, head, cmd_cnt);
	print_struct(cmds, cmd_cnt);
	return (true);
}
