/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 14:12:14 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/17 19:47:04 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (type != QUOTE_ARG)
	{
		while (type == OPER && input[len] && incl_char(input[len], " >|<"))
			len++;
		while (input[len] && !incl_char(input[len], " >|<"))
		{
			len++;
			if (incl_char(input[len], "\'\""))
				len += cnt_len_between(input, input[len], len + 1) + 2;
		}
		len -= start;
	}
	if (type == QUOTE_ARG)
		len = cnt_len_between(input, input[start], start + 1) + 2;
	res = ft_substr(input, start, len);
	input = del_substr(input, start, len);
	if (!res || !input)
		return (NULL);
	return (res);
}

/**
 * @brief Creating a node & adding it to the list
 * @param str_tok a single token from the input string
 * @param type defines the token's meaning
 * 
 * @note Both creates the first node and adds nodes to the first
*/
int	add_tok(char *str_tok, t_args **head, t_type_tok type)
{
	t_args	*new;
	t_args	*temp;

	temp = *head;
	new = malloc(sizeof(t_args));
	if (!new)
		return (-1);
	new->arg = str_tok;
	new->type = type;
	new->next = NULL;


	if (str_tok[0] == '|' && type == OPER)
		new->type = CMD;
	if (incl_char(str_tok[0], "><") && type == OPER)
		new->type = REDIRECT;
	
	
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
			i = add_tok(get_tok(input, i, OPER), &head, OPER);
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