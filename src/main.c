/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 10:37:57 by verdant           #+#    #+#             */
/*   Updated: 2023/03/15 13:20:01 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	debug_msg(char *str)
{
	ft_printf("%s", str);
	return (1);
}


/**
 * @brief Counts the length of characters between quotes
 * 
 * @note Usecase: When I want to skip over the characters between quotes
*/
int	cnt_len_between(char *str, char c, int index)
{
	int		i;

	i = 0;
	while (str[index])
	{
		if (str[index] == c)
			return (i); // Does this work? If not use break ; here
		index++;
		i++;
	}
	return (-1);
}


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
	while (type == 0 && input[len] && incl_char(input[len], " >|<"))
		len++;
	while (input[len] && !incl_char(input[len], " ><"))
		len++;
	len -= start;
	if (type == 2)
		len = cnt_len_between(input, input[start], start + 1) + 2;
	res = ft_substr(input, start, len);
	input = del_substr(input, start, len);
	if (!res || !input)
		return (NULL);
	return (res);
}

// "test HOME"


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
	if (!temp)
	{
		*head = new;
		new->next = NULL;
		return (0);
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	return (0);
}

t_args	*prep_tok(char *input, t_args *head)
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
			i = add_tok(get_tok(input, i, QUOTE), &head, QUOTE);
		if (i == -1)
			return (NULL);
		i++; // Skip spaces
	}
	return (head);
}

			// ft_printf("%d", cnt_len_between(input, input[i], i + 1));
// "tok 1 tok2"

void	tokenizer(char *input, t_args *head)
{
	int i;

	i = 0;
	head = prep_tok(input, head);
	t_args *temp = head;
	while (temp != NULL)
	{
		ft_printf("%s\t", temp->arg);
		ft_printf("%d\n", temp->type);
		temp = temp->next;
	}
}


int	main(int argc, char *argv[])
{
	// With strtrim it will proab. cause mem leak
	char	*input = ft_strtrim(readline(""), " ");  // Reading the cmd line input
	// char	*input = "ls libft | echo \"||| test\""; // When Debugging
	
	t_args *head;

	if (!input || !are_quotes_even(input))
		return (debug_msg("main: input or quotes\n"));
	tokenizer(input, head);
	return (EXIT_SUCCESS);
}


