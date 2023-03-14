/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 15:13:57 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/13 13:06:03 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "testing.h"
#include <strings.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>



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

int	count_wrds(char *line, char *delim_set, char *skip)
{
	int	i;
	int	cnt;

	i = 0;
	cnt = 0;
	if (line == NULL)
		return (-1);

	while (line[i])
	{
		// Reached a non-delim character => a word
		if (!incl_char(line[i], delim_set))
		{
			cnt++;
			// Iterate over the word until a delim or a quotation mark occurs
			while (!incl_char(line[i], "|\'\"") && line[i + 1] != '\0')
				i++;
			// Iterate over characters between quotes
			if (incl_char(line[i], skip))
			{
				i++;
				while (line[i + 1] != '\0' && !incl_char(line[i], skip))
					i++;
			}
		}
		i++;
	}
	return (cnt);
}

// I'm facing the problem that I have a pipe inside of quotes
// Then I need to skip those chars until the end quote is
// reached. I thought I already fixed that but somehow I'm men
// tally stuck at that point right now

int	count_chars(char *line, char *delim_set, char *skip)
{
	int	cnt;
	int	i;

	cnt = 0;
	i = 0;
	if (!line)
		return (-1);
	// // Skip over delimiters
	while (line[i] && incl_char(line[i], delim_set)) 
		i++;
	if (incl_char(line[i], skip)) // cnt chars until next skip char
	{
		i++;
		while (line[i] && !incl_char(line[i], skip))
		{
			i++;
			cnt++;
		}
		return (cnt);
	}
	// cnt chars until next delimiter
	while (line[i] && !incl_char(line[i], delim_set))
	{
		i++;
		cnt++;
	}
	return (cnt);
}

char *store_wrd(char *line, char *delim_set, char *skip)
{
	int	i;
	int	k;
	const int char_cnt = count_chars(line, delim_set, skip);
	
	char *temp;

	i = 0;
	k = 0;
	temp = malloc(sizeof(char) * (char_cnt + 1));
	if (!temp || char_cnt == -1)
		return (NULL);
		
	temp[char_cnt] = '\0';
	// Skipping over delimiters and skips chars
	while (line[i] && (incl_char(line[i], delim_set) || incl_char(line[i], skip)))
		i++;
	// Copying the chars over
	while (line[i] && k < char_cnt)
		temp[k++] = line[i++];
	return (temp);
}

void	trim_string(char *line, char *delimiters, char *skip)
{
	int	i;
	int	k;
	
	i = 0;
	k = 0;
	while (line[i] && (incl_char(line[i], delimiters) || incl_char(line[i], skip)))
		i++;
	i += count_chars(line, delimiters, skip);
		i++;
	while (line[i] && incl_char(line[i], delimiters))
		i++;
	while (line[i])
		line[k++] = line[i++];
	line[k] = '\0';
}

char **ft_split_ultimate(char *line, char *delim_set, char *skip)
{
	const int	wrd_cnt = count_wrds(line, delim_set, skip);
	char **str_arr;
	int i;
	
	printf("%d", wrd_cnt);

	

	// str_arr = malloc(sizeof(char *) * (wrd_cnt + 1));
	// if (!str_arr || wrd_cnt == -1)
	// 	return (free(str_arr), NULL);
	// str_arr[wrd_cnt] = NULL;
	// i = 0;
	// while (i < wrd_cnt)
	// {
	// 	str_arr[i] = store_wrd(line_trimable, delim_set, skip);
	// 	// if (str_arr[i] == NULL) // Do we need this?
	// 		// free_mem(str_arr); 
	// 	trim_string(line_trimable, delim_set, skip);
	// 	i++;
	// }
	return (NULL); // str_arr
}


int main(void)
{
	char *line = "0123 \"01|34\"abc def| ls";
	// printf("%c\n", line[4]);
	// printf("%c\n", line[10]);
	// printf("%c\n", line[12]);
	// printf("Line:\t%s\n", line);
	char **arr = ft_split_ultimate(line, "|", "\'\"");
	// printf("%s\n", arr[0]);
	// printf("%s\n", arr[1]);
}