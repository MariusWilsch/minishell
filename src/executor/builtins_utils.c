/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dickklouwer <tklouwer@student.codam.nl>      +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/21 10:38:52 by dickklouwer   #+#    #+#                 */
/*   Updated: 2023/05/22 11:07:35 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	check_flag(char *argv[])
{
	if (!argv[1])
		return (true);
	if (ft_strncmp(argv[1], "-n", 2) == 0 && ft_strlen(argv[1]) == 2)
		return (false);
	return (true);
}