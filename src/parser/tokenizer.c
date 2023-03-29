/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwilsch <mwilsch@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/15 14:12:14 by mwilsch       #+#    #+#                 */
/*   Updated: 2023/03/29 16:41:54 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Cutting out the token out of the input string
 * 
 * @note I add 2 to len because of the quotes
 * Could I refactor get_tok?
*/
char	*get_tok(char *input, int start, t_type_tok type)
{
	char	*res;
	int		len;

	len = start;
	while (type == OPERATOR && input[len] && incl_char(input[len], " >|<"))
		len++;
	while (type != QUOTE_ARG && input[len] && !incl_char(input[len], " >|<"))
	{
		if (incl_char(input[len], "\'\""))
			len += cnt_len_between(input, input[len], len + 1) + 1;
		len++;
	}
	len -= start;
	if (type == QUOTE_ARG)
		len = cnt_len_between(input, input[start], start + 1) + 2;
	res = ft_substr(input, start, len);
	input = del_substr(input, start, len);
	if (!res || !input)
		return (NULL);
	return (res);
}

/**
 * 
*/
t_args	*create_node(char *str, t_type_tok type)
{
	t_args	*new;

	new = malloc(sizeof(t_args));
	if (!new)
		return (NULL);
	new->arg = str;
	new->err_tok = OK;
	new->next = NULL;
	new->type = type;
	new->cmnd_count = 0;
	if (type == OPERATOR && incl_char(str[0], "|."))
	{
		new->type = CMD;
		if (ft_strchr(str, '>') || ft_strchr(str, '<'))
		{
			str = del_substr(str, 0, cnt_occur(str + 1, ' ') + 1);
			new->type = REDIRECT;
		}
	}
	if (incl_char(str[0], "><") && type == OPERATOR)
		new->type = REDIRECT;
	return (new);
}

/**
 * @brief Creating a node & adding it to the list
 * @param str a single token from the input string
 * @param type defines the token's meaning
 * 
 * @note Both creates the first node and adds nodes to the first
*/
int	add_tok(char *str, t_args **head, t_type_tok type)
{
	t_args	*new;
	t_args	*temp;

	temp = *head;
	new = create_node(str, type);
	if (!new)
		return (-1);
	if (!temp) // This marks the first node
	{
		*head = new;
		if (type == ARG)
			new->type = CMD;
		new->next = NULL;
		return (0);
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	return (0);
}


/**
 * @brief Adds all token to a linked list
 * 
 * @param input Command Line Input
*/
t_args	*create_tok_list(char *input, t_args *head)
{
	int	i;

	i = 0;
	while (ft_strlen(input) != 0 && input[i])
	{
		if (incl_char(input[i], ".>|<"))
			i = add_tok(get_tok(input, i, OPERATOR), &head, OPERATOR);
		if (ft_isalnum(input[i]) || incl_char(input[i], "$-"))
			i = add_tok(get_tok(input, i, ARG), &head, ARG);
		if (incl_char(input[i], "\'\""))
			i = add_tok(get_tok(input, i, QUOTE_ARG), &head, QUOTE_ARG);
		if (i == -1)
			return (NULL);
		i++;
	}
	return (head);
}

/**
 * 
*/
t_args	*process_tok(t_args *head)
{
	t_args *node = head;

	while (node != NULL)
	{
		if (node->type == CMD && !is_builtin(node))
			node->arg = resolute_cmd(node, ft_strdup(node->arg));
		if (node->type == REDIRECT && check_redirect(node->arg, node->arg[0], cnt_occur(node->arg, node->arg[0]), node) > 0)
		{
			head->type = REPROMPT;
			return (head); // what if !str
		}
		if (ft_strchr(node->arg, '$') && node->arg[0] != '\'')
		{
			while (ft_strchr(node->arg, '$') && node->type != REDIRECT)
				node->arg = sub_env(node->arg, get_env_len(node->arg));
		}
		del_quotes(node->arg);
		node = node->next;
	}
	return (head);
}
