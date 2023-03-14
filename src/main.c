/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 10:37:57 by verdant           #+#    #+#             */
/*   Updated: 2023/03/14 16:54:53 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	debug_msg(char *str)
{
	ft_printf("%s", str);
	return (1);
}


/**
 * @brief Checks if quotes are opened and closed
 * 
 * @param input command line input
 * 
 * @note Quotes inside of quotes are not considered
*/
bool are_quotes_even(char *input)
{
	bool	flag;
	char	c;
	int		i;

	i = 0;
	flag = true;
	while (input[i])
	{
		if (!flag && input[i] == c)
		{
			flag = true;
			i++;
		}
		if (flag && incl_char(input[i], "\'\""))
		{
			c = input[i];
			flag = false;
		}
		if (input[i])
			i++;
	}
	return (flag);
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
 * @brief Deletes a substr from a str
 * 
 * @param start The starting index of str to delete from
 * @param len The amount of chars to delete
 * 
 * @note I could rewrite this function to be a bool and use a double ptr
 * for char *str so that I do not lose the changes outside local scope, however
 * this would entail that I have to rewrite my env_subsitution to accomodate
 * for that
 * 
 * @note Benefits: 1. I can check if del_substr went right or not 2. I can 
 * save lines
*/
char	*del_substr(char *str, int start, int len)
{
	int	copy_from;

	if (!str)
		return (NULL);
	if (len == 0)
		return (str);
	copy_from = start + len;
	while (str[copy_from])
		str[start++] = str[copy_from++];
	str[start] = '\0';
	return (str);
}

char	*cut_tok(char *input, int start, int type)
{
	char	*res;
	int		len;

	len = start;
	while (type == 0 && input[len] && incl_char(input[len], " ><"))
		len++;
	while (input[len] && !incl_char(input[len], " ><"))
		len++;
	len -= start;
	if (type == 2)
		len = cnt_len_between(input, input[start], start + 1);
	res = ft_substr(input, start, len);
	input = del_substr(input, start, len);
	if (!res || !input)
		return (NULL);
	return (res);
}

char *get_tok(char *input)
{
	int i;

	i = 0;
	while (input[i])
	{
		if (incl_char(input[i], ">|<"))
			return (cut_tok(input, i, 0));
		if (ft_isalnum(input[i]) || incl_char(input[i], "$"))
			return (cut_tok(input, i, 1));
		if (incl_char(input[i], "\'\""))
			return (cut_tok(input, i, 2));
		i++; // Skip spaces
	}
	return (NULL);
}

			// ft_printf("%d", cnt_len_between(input, input[i], i + 1));
// "tok 1 tok2"

void	tokenizer(char *input)
{
	char *tok;
	int i;

	i = 0;
	tok = get_tok(input);
	printf("%s", input);
}


int	main(int argc, char *argv[])
{
	// With strtrim it will proab. cause mem leak
	char	*input = ft_strtrim(readline(""), " ");  // Reading the cmd line input
	// char	*input = "ls libft | echo \"||| test\""; // When Debugging
	// printf("|%s|\n", input);
	if (!input || !are_quotes_even(input))
		return (debug_msg("main: input or quotes\n"));
	tokenizer(input);
	return (EXIT_SUCCESS);
}


