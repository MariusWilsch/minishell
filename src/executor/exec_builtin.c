/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: verdant <verdant@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/29 14:00:08 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/04/20 12:08:52 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     exec_builtin(char *func, int argc, char **argv)
{
    if (ft_strncmp("echo", func, 4) == 0)
        echo(argc, argv);
    if (ft_strncmp("cd", func, 2) == 0)
        cd(argc, argv[1]);
    if (ft_strncmp("pwd", func, 3) == 0)
        pwd();
    if (ft_strncmp("unset", func, 5) == 0)
        unset(argc, argv);
    if (ft_strncmp("env", func, 3) == 0)
        env();
    if (ft_strncmp("exit", func, 4) == 0)
        exit(0);
    return (EXIT_SUCCESS);
}