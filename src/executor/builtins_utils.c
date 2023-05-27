/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 10:38:52 by dickklouwer       #+#    #+#             */
/*   Updated: 2023/05/27 17:57:47 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	check_flag(char *argv[], int *idx)
{
	if (!argv[1])
		return (true);
	if (ft_strncmp(argv[1], "-n", 2) == 0 && ft_strlen(argv[1]) == 2)
	{
		while (argv[*idx] && ft_strcmp(argv[*idx], "-n") == 0)
			(*idx)++;
		return (false);
	}
	return (true);
}

// If -n is the first argument we want to skip the newline
// If -n is the first argument and there more -n after the first one we skip those too