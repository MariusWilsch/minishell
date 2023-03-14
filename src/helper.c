/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 11:28:28 by verdant           #+#    #+#             */
/*   Updated: 2023/03/13 13:28:52 by verdant          ###   ########.fr       */
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
 * @brief Counts occurences of char c inside of a string
 * 
 * @param str String to search occurence in
 * @param c the character to search for
 * 
 * @note Does not count the occurence if the char c is inside of 
 * quotations
 */
int	cnt_occur_skip(char *str, char c, char *skip)
{
	int	i;
	int	cnt;
	
	i = 0;
	cnt = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (incl_char(str[i], skip)) // Could be it's own int function i += skip()
		{
			i++;
			while (!incl_char(str[i], skip) && str[i + 1] != '\0')
				i++;
		}
		if (str[i] == c)
			cnt++;
		i++;
	}
	return (cnt);
}





/**
 * @brief Checks if quotes are closed and if there only single pipes
 * 
 * @param str Unproccsed command line input
 * @return true or false
 */
bool	check_input(const char *str)
{
	int	i;
	int	cnt_single;
	int	cnt_double;

	cnt_single = 0;
	cnt_double = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			cnt_single++;
		if (str[i] == '\"')
			cnt_double++;
		if (str[i] == '|' && str[i + 1] == '|')
				return (false);
		i++;
	}
	return ((cnt_single % 2) == 0 && (cnt_double % 2) == 0);
}