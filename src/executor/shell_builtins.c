/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_builtins.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/02/07 14:05:40 by tklouwer      #+#    #+#                 */
/*   Updated: 2023/03/29 13:59:15 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int echo(int argc, char **argv)
{
    int i;

    i = 1;
    rl_on_new_line();
    while (i < argc)
    {
        printf("%s", argv[i]);
        if (i != argc - 1)
            printf(" ");
        i++;
    }
    printf("\n");
    return (i);
}

int cd(int argc, char **argv)
{
    if (!argc || !argv)
        return (perror("chdir"), 1);
    if (chdir(argv[1]) != 0) 
        perror("chdir");
  return (EXIT_SUCCESS);
}

int pwd(char *input)
{
    char *cwd;

    cwd = getcwd(input, 1024);
    if (!cwd)
        return (perror("getcwd"), 1);
    ft_putendl_fd(cwd, 1);
    return (EXIT_SUCCESS);
}

int sh_exit(int status)
{
    exit(status);
}

int env()
{    
    extern char **environ;
    char **envp;

    envp = environ;
    while (*envp != NULL) 
    {
        printf("%s\n", *envp);
        envp++;
    }
    envp = NULL;
    return (EXIT_SUCCESS);
}

void unset(const char *input) 
{
    extern char **environ;
    int len;
    int i;
    int j;

    len = ft_strlen(input);
    i = 0;
    while (environ[i])
    {
        if (ft_strncmp(environ[i], input, len) == 0 && environ[i][len] == '=') 
        {
            j = i;
            while (environ[j])
            {
                environ[j] = environ[j + 1];
                j++;
            }
            break;
        }
        i++;
    }
}