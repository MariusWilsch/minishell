/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pre_executor.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwilsch <mwilsch@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/24 11:27:54 by mwilsch       #+#    #+#                 */
/*   Updated: 2023/03/24 14:57:36 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

bool operation_init(t_operation *oper, char *str, t_err_tok err_type)
{
	int cnt;
	const int file_length = ft_strlen(str) - cnt;
	const char c = str[0];
	
	cnt = cnt_occur(str, str[0]);
	oper->redirect = NULL;
	oper->filename = NULL;
	if (err_type == ENV_REDIRECT_ERR)
		return (oper->type = ERR, true);
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

/**
 * @note FOR EVERY COMMAND AN INDEPENT STRUCT WILL BE CREATED AND ADDED TO THE ARRAY.
*/
t_args	*init_members(t_cmds *cmd, t_args *head, int argc, int operc)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	cmd->cmd = head->arg;
	cmd->oper = NULL;
	cmd->args = malloc(sizeof(char *) * (argc + 1));
	if (operc != 0)
		cmd->oper = malloc(sizeof(t_operation) * operc);
	if (!cmd->args || (operc > 0 && !cmd->oper))
		return (NULL);
	cmd->args[argc] = NULL;
	if (argc == 0 && operc == 0)
		return (head->next);
	while (head != NULL && (head->type != CMD || head->type != BUILT_IN))
	{
		if (i < argc && (head->type == ARG || head->type == QUOTE_ARG))
			cmd->args[i++] = head->arg;
		if (k < operc && head->type == REDIRECT)
			operation_init(&cmd->oper[k++], head->arg, head->err_tok);
		head = head->next;
		if (i == argc && k == operc)
			break;
	}
	return (head);
}
/**
 * @note ARGUMENTS ARE BEING COUNTED. 
*/
t_cmds	*create_structs(t_args *node, t_cmds *cmds, t_args *head, int cmd_cnt)
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
		head = init_members(&cmds[i++], head, arg_cnt, operator_cnt);
		if (node != NULL)
			node = node->next;
	}
	return (cmds);
}
/**
 * 
 * @note HELPS WITH TESTING THE CODE
*/
void	print_struct(t_cmds *cmds, int cmd_limit)
{
	int i = 0;
	int k = 0;
	int	cmd_cnt = 0;
	int	cnt_oper;

	while (cmd_cnt < cmd_limit)
	{
		cnt_oper = 0;
		ft_printf("cmd name: %s\n", cmds[cmd_cnt].cmd);
		for (i = 0; cmds[cmd_cnt].args[i] != NULL; i++)
			ft_printf("args: %s\n", cmds[cmd_cnt].args[i]);
		if (cmds[cmd_cnt].oper != NULL)
		{
			// add scanf func here to make it dynamic
			k = 0;
			while (k < 1)
			{
				ft_printf("red: |%s|\t", cmds[cmd_cnt].oper[k].redirect);
				ft_printf("filename: |%s|\t", cmds[cmd_cnt].oper[k].filename);
				ft_printf("type: %d\n", cmds[cmd_cnt].oper[k].type);
				k++;
			}
		}
		ft_printf("\n");
		cmd_cnt++;
	}
}

/**
 * 
 * @note What if there there is no cmd but a redirect as the first string
*/
bool	init_structs(t_args *head)
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
	node = head;
	create_structs(node, cmds, head, cmd_cnt);
	print_struct(cmds, cmd_cnt);
	return (true);
}
