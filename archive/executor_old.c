/* ************************************************************************** */
/*                                                                            */
<<<<<<< HEAD
/*                                                        ::::::::            */
/*   executor_old.c                                     :+:    :+:            */
=======
<<<<<<<< HEAD:archive/executor_old.c
/*                                                        :::      ::::::::   */
/*   executor_old.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 17:41:28 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/24 11:37:29 by mwilsch          ###   ########.fr       */
========
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
>>>>>>> f5e44fdb8c88e462cafdd2fca5360f53c4200184
/*                                                     +:+                    */
/*   By: tklouwer <tklouwer@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/22 16:40:17 by tklouwer      #+#    #+#                 */
<<<<<<< HEAD
/*   Updated: 2023/03/24 12:39:07 by tklouwer      ########   odam.nl         */
=======
/*   Updated: 2023/03/24 13:13:30 by tklouwer      ########   odam.nl         */
>>>>>>>> f5e44fdb8c88e462cafdd2fca5360f53c4200184:src/executor.c
>>>>>>> f5e44fdb8c88e462cafdd2fca5360f53c4200184
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


/* 
    Parse the command line input to identify the redirects and the corresponding input and output files.
    Handle any here-documents (<<) by creating temporary files and writing the content to them.
    Use fork() to create a child process for each command or pipe stage.
    In each child process, perform the necessary input and output redirections using dup2():
    If the command reads from a file or here-document, redirect stdin (file descriptor 0) to the appropriate file using dup2().
    If the command writes to a file, redirect stdout (file descriptor 1) to the appropriate file using dup2().
    If the command is part of a pipeline, redirect stdin and/or stdout to the corresponding read and write ends of the pipes using dup2().
    Close any unused file descriptors in the child process.
    Execute the command in the child process using an exec() family function (e.g., execl(), execv(), execvp(), etc.).
    In the parent process, wait for the child processes to complete using wait() or waitpid().
 */

int structlen(t_cmds *head)
{
    int i;

    i = 0;
    while (head[i].cmd_path)
        i++;
    return (i);
}
int redirect(char *cmd)
{
<<<<<<< HEAD
	int	i;
	int	arg_cnt;
	int	operator_cnt;
	
	i = 0;
	if (node->type == CMD || node->type == BUILT_IN)
		node = node->next;
	while (i < cmd_cnt)
	{
=======
    if (ft_strnstr(cmd, ">>", 2))
        return (1);
    if (ft_strnstr(cmd, ">", 1))
        return (2);
    if (ft_strnstr(cmd, "<<", 2))
        return (3);
    if (ft_strnstr(cmd, "<", 1))
        return (4);
}
void	execute_command(t_cmds *head)
{
	if (!head->cmd_path)
	{
		perror("Command not found");
	}
	if (execve(head->cmd_path, head->args, NULL) == -1)
	{
<<<<<<<< HEAD:archive/executor_old.c
>>>>>>> f5e44fdb8c88e462cafdd2fca5360f53c4200184
		arg_cnt = 0;
		operator_cnt = 0;
		while (node != NULL && node->type != CMD && node->type != BUILT_IN)
		{
			if ((node->type == ARG || node->type == QUOTE_ARG))
				arg_cnt++;
			if (node->type == REDIRECT)
				operator_cnt++;
			node = node->next;
		}
		head = fill_struct(&cmds[i++], head, arg_cnt, operator_cnt);
		if (node != NULL)
			node = node->next;
<<<<<<< HEAD
	}
	return (cmds);
}
void	execute_command(t_cmds *head)
{
	if (!head->cmd_path)
	{
		perror("Command not found");
	}
	if (execve(head->cmd_path, head->args, NULL) == -1)
	{
		perror("Execve failed");
		printf("cmd name: %s\n", cmds[cmd_cnt].cmd);
		for (i = 0; cmds[cmd_cnt].args[i] != NULL; i++)
			printf("args: %s\n", cmds[cmd_cnt].args[i]);
		for (k = 0; cmds[cmd_cnt].oper[k] != NULL; k++)
			printf("oper_args: %s\n", cmds[cmd_cnt].oper[k]);
		printf("\n");
		cmd_cnt++;
	}
}
int executor(t_cmds *head)
{
    int     num_commands;
=======
========
		perror("Execve failed");
>>>>>>>> f5e44fdb8c88e462cafdd2fca5360f53c4200184:src/executor.c
	}
}
int executor(t_args *head)
{

<<<<<<<< HEAD:archive/executor_old.c
	while (cmd_cnt < cmd_limit)
	{
		ft_printf("cmd name: %s\n", cmds[cmd_cnt].cmd);
		for (i = 0; cmds[cmd_cnt].args[i] != NULL; i++)
			ft_printf("args: %s\n", cmds[cmd_cnt].args[i]);
		for (k = 0; cmds[cmd_cnt].oper[k] != NULL; k++)
			ft_printf("oper_args: %s\n", cmds[cmd_cnt].oper[k]);
		ft_printf("\n");
		cmd_cnt++;
	}
========
>>>>>>>> f5e44fdb8c88e462cafdd2fca5360f53c4200184:src/executor.c
}

int executor2()
{
        int     num_commands;
>>>>>>> f5e44fdb8c88e462cafdd2fca5360f53c4200184
    int     i;
    int     pipe_fd[2 * num_commands];
    int     redirect_case;
    pid_t   child_prcs;

<<<<<<< HEAD
=======
<<<<<<<< HEAD:archive/executor_old.c
/**
 * 
 * @note 1. Quote ARG are not working yet
 * @note 2. Err Toks for CMD and ARGS do not matter but for oper they do. 
 * @note 2.1 I either change the env_var_err thing to do in the executor or add the err tok to the struct
 * @note 2.2 If I add it to the struct it would be best if change the char **oper to a small linked list
*/
bool	executor(t_args *head)
========
>>>>>>> f5e44fdb8c88e462cafdd2fca5360f53c4200184
    i = 0;
    while (i < num_commands)
    {
        if (pipe(pipe_fd + 2 * i) == -1)
        {
            perror("pipe");
            return (EXIT_FAILURE);
        }
    }
    i = 0;
    while (i < num_commands)
    {
        child_prcs = fork();
        if (child_prcs < 0)
            perror("Fork:");
        else if (child_prcs == 0)
            child_process(head);
    }
}
int child_process(t_cmds *head)
<<<<<<< HEAD
=======
>>>>>>>> f5e44fdb8c88e462cafdd2fca5360f53c4200184:src/executor.c
>>>>>>> f5e44fdb8c88e462cafdd2fca5360f53c4200184
{
    int     redirect_case;
    int     outfile;

    redirect_case = redirect(head->cmd_path);
    if (redirect_case == 1 || redirect_case == 2)
    {
        if (redirect_case == 1)
            outfile = open(head->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            outfile = open(head->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outfile == -1) 
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(outfile, STDOUT_FILENO);
        close(outfile);
    }
    return (EXIT_SUCCESS);
}
