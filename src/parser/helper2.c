/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   helper2.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: verdant <verdant@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 16:59:02 by verdant       #+#    #+#                 */
/*   Updated: 2023/05/22 11:45:02 by tklouwer      ########   odam.nl         */
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

// void create_delimiter_node(t_args *temp)
// {
// 	t_args *new;

// 	new = malloc(sizeof(t_args));
// 	if (!new)
// 		return ;
// 	new->arg = "|";
// 	new->err_tok = OK;
// 	new->next = NULL;
// 	new->prev = NULL;
// 	temp->next = new;
// 	new->prev = temp;
// 	temp = temp->next;
// }
