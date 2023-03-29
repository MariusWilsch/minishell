/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/29 14:00:08 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/03/29 14:06:04 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     exec_builtin(char *func, int argc, char **argv)
{
    if (ft_strncmp("echo", func, 4) == 0)
        echo(argc, argv);
    if (ft_strncmp("cd", func, 4) == 0)
        cd(argc, *argv);
    if (ft_strncmp("pwd", func, 4) == 0)
        pwd(*argv);
    if (ft_strncmp("unset", func, 4) == 0)
        unset(*argv);
    if (ft_strncmp("env", func, 4) == 0)
        env();
    return (EXIT_SUCCESS);
}