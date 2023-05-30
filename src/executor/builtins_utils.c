/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 10:38:52 by dickklouwer       #+#    #+#             */
/*   Updated: 2023/05/30 11:29:12 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	check_flag(char *argv[], int *idx)
{
	int	i;
	
	if (!argv[1])
		return (true);
	if (ft_strncmp(argv[1], "-n", 2) == 0 && ft_strlen(argv[1]) == 2)
	{
		i = 0;
		while (argv[*idx])
		{
			while (argv[*idx][i] && incl_char(argv[*idx][i], "-n"))
				i++;
			if (i == ft_strlen(argv[*idx]))
				(*idx)++;
			else
				break ;
		}
		return (false);
	}
	return (true);
}

// If -n is the first argument we want to skip the newline
// If -n is the first argument and there more -n after the first one we skip those too