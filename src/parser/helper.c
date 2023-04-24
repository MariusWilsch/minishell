/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 11:28:28 by verdant           #+#    #+#             */
/*   Updated: 2023/04/19 15:00:40 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lstsize_shell(t_env *env)
{
	size_t	n;

	n = 0;
	while (env != NULL)
	{
		env = env->next;
		n++;
	}
	return (n);
}

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
*/
char	*del_substr(char *str, int start, int len)
{
	int	copy_from;

	if (!str)
		return (NULL);
	if (len == 0 || len > ft_strlen(str))
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

/**
 * @brief Deletes quotes from strings when they are not needed anymore
 * 
 * @note Does not malloc
 * @note test with: echo "pla '$HOME"bra"'bra"
*/
char	*del_quotes(char *str)
{
	int		cnt;
	int		i;
	char	c;

	if (!str)
		return (NULL);
	if (!ft_strchr(str, '\'') && !ft_strchr(str, '\"'))
		return (str);
	cnt = 2;
	c = '\"';
	if (ft_strchr(str, '\''))
		c = '\'';
	while (cnt)
	{
		i = ft_strclen(str, c);
		str = del_substr(str, i, 1);
		cnt--;
	}
	return (str);
}

int	cnt_occur(char *str, char c)
{
	int	i;
	
	i = 0;
	if (!str)
		return (-1);
	while (str[i] && str[i] == c)
		i++;
	return (i);
}
