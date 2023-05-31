/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 10:38:52 by dickklouwer       #+#    #+#             */
/*   Updated: 2023/05/31 14:17:54 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	check_flag(char **argv, int *i)
{
	int		idx;
	int		idx_2;
	bool	flag;

	idx = 1;
	if (!argv[1])
		return (true);
	flag = true;
	while (argv[idx] != NULL)
	{
		idx_2 = 0;
		while (argv[idx][idx_2] && incl_char(argv[idx][idx_2], "-n"))
			idx_2++;
		if (argv[idx][idx_2] == '\0')
		{
			(*i)++;
			flag = false;
		}
		else
			break ;
		idx++;
	}
	return (flag);
}
