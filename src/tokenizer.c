/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 14:12:14 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/18 16:28:33 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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


t_args	*create_node(char *str, t_type_tok type)
{
	t_args *new;

	new = malloc(sizeof(t_args));
	if (!new)
		return (NULL);
	new->arg = str;
	new->err_tok = OK;
	new->next = NULL;
	new->type = type;
	if (str[0] == '|' && type == OPERATOR)
		new->type = CMD;
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
	bool	flag;

	i = 0;
	while (ft_strlen(input) != 0 && input[i])
	{
		if (incl_char(input[i], ">|<"))
			i = add_tok(get_tok(input, i, OPERATOR), &head, OPERATOR);
		if (ft_isalnum(input[i]) || incl_char(input[i], "$-"))
			i = add_tok(get_tok(input, i, ARG), &head, ARG);
		if (incl_char(input[i], "\'\""))
			i = add_tok(get_tok(input, i, QUOTE_ARG), &head, QUOTE_ARG);
		if (i == -1)
			return (NULL);
		if (input[i] == ' ')
			i++; // Skip spaces
	}
	return (head);
}