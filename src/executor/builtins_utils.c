/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_builtins_utils.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dickklouwer <tklouwer@student.codam.nl>      +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/21 10:38:52 by dickklouwer   #+#    #+#                 */
/*   Updated: 2023/05/21 11:10:36 by dickklouwer   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	check_flag(char *argv[])
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (ft_strncmp(argv[i], "-n", 2) == 0 && ft_strlen(argv[i]) == 2)
			return (false);
		i++;
	}
	return (true);
}