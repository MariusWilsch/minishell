/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 11:28:28 by verdant           #+#    #+#             */
/*   Updated: 2023/03/15 14:31:31 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a char exits inside of a string
 * 
 * @param c char of string
 * @param search_str the characters to compare c against
 */
bool	incl_char(char c, char *search_str)
{
	int	i;

	i = 0;
	if (!search_str)
		return (false);
	while (search_str[i])
	{
		if (c == search_str[i])
			return (true);
		i++;
	}
	return (false);
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


/**
 * @brief Counts the length of characters between quotes
 * 
 * @param 
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
