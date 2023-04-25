/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:59:02 by verdant           #+#    #+#             */
/*   Updated: 2023/04/25 09:01:22 by verdant          ###   ########.fr       */
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
		if (env->hidden == false)
			n++;
	}
	return (n);
}


void	free_list(t_args *head)
{
	while (head != NULL)
	{
		free(head->arg);
		head = head->next;
	}
	free(head);
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