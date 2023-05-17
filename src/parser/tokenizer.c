/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 14:12:14 by mwilsch           #+#    #+#             */
/*   Updated: 2023/05/17 16:25:48 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Cutting out the token out of the input string
 * 
 * @note I add 2 to len because of the quotes
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

t_args	*create_node(char *str, t_type_tok type)
{
	t_args	*new;

	new = malloc(sizeof(t_args));
	if (!new)
		return (NULL);
	new->arg = str;
	new->err_tok = OK;
	new->next = NULL;
	new->prev = NULL;
	new->type = type;
	if (type == OPERATOR && incl_char(str[0], "|."))
	{
		new->type = CMD;
		if (ft_strchr(str, '>') || ft_strchr(str, '<'))
		{
			str = del_substr(str, 0, cnt_occur(str + 1, ' ') + 1);
			new->type = REDIR;
		}
	}
	if (incl_char(str[0], "><") && type == OPERATOR)
		new->type = REDIR;
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
	if (!temp)
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
	new->prev = temp;
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
		if (incl_char(input[i], ">|<"))
			i = add_tok(get_tok(input, i, OPERATOR), &head, OPERATOR);
		if (ft_isalnum(input[i]) || incl_char(input[i], ".$-/_+-*"))
			i = add_tok(get_tok(input, i, ARG), &head, ARG);
		if (incl_char(input[i], "\'\""))
			i = add_tok(get_tok(input, i, QUOTE_ARG), &head, QUOTE_ARG);
		if (i == -1)
			return (NULL);
		i++;
	}
	return (head);
}

bool	cmd_after_redirect(t_args *node)
{
	if (node->prev == NULL)
		return (false);
	if (node->prev->type == REDIR && node->type == ARG)
		return (true);
	return (false);
}

t_args	*process_tok(t_args *head, char *input)
{
	t_args	*n;

	n = head;
	while (n != NULL)
	{
		if ((n->type == CMD || cmd_after_redirect(n)) && !is_builtin(n))
		{
			n->arg = resolute_cmd(n, ft_strdup(n->arg));
			n->type = CMD;
		}
		if (n->type == REDIR
			&& c_red(n->arg, cnt_occur(n->arg, n->arg[0]), n) > 0)
			return (head->type = REPROMPT, free_list(head), free(input), NULL);
		if (ft_strchr(n->arg, '$') && n->arg[0] != '\'')
		{
			while (ft_strcmp(n->arg, "$?") != 0 && ft_strchr(n->arg, '$')
				&& n->type != REDIR)
				n->arg = sub_env(n->arg, get_env_len(n->arg));
		}
		del_quotes(n->arg);
		n = n->next;
	}
	return (head);
}
