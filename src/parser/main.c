/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: verdant <verdant@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/10 10:37:57 by verdant       #+#    #+#                 */
/*   Updated: 2023/05/20 12:28:54 by dickklouwer   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	leaks(void)
// {
// 	system("leaks -q minishell");
// }

void	free_list(t_args *head)
{
	t_args	*temp;

	while (head != NULL)
	{
		free(head->arg);
		temp = head;
		head = head->next;
		free(temp);
	}
	free(head);
}

char	*prompt(char *str)
{
	char	*input;

	input = NULL;
	if (!str)
		exit(1);
	if (!str[0])
		return (str);
	input = ft_strtrim(str, " ");
	free(str);
	if (!input)
		return (NULL);
	if (!input[0] || !are_quotes_even(input) || !is_valid_pipe(input))
	{
		free(input);
		return (NULL);
	}
	if (input[0] != '\0')
		add_history(input);
	return (input);
}

int	minishell(t_args *head, char **envp)
{
	char	*input;
	t_env	*env_l;

	env_l = NULL;
	while (1)
	{
		head = NULL;
		input = prompt(readline("Minishell-1.0$ "));
		if (!input)
			continue ;
		if (ft_strncmp(input, "$?", 2) == 0)
		{
			ft_printf("%d\n", g_status);
			continue ;
		}
		head = create_tok_list(input, head);
		head = process_tok(head, input);
		if (head == NULL)
			continue ;
		env_init(&env_l, envp);
		executor(head, &env_l);
		free(input);
		free_list(head);
	}
	return (EXIT_SUCCESS);
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		if (wait(&g_status) == -1)
			rl_redisplay();
	}
}

/**
 * @note Ctlr C in Parent is just reprompt (custome handler)
 * @note Ctlr C in child process is default behaviour (SIGDFL)
 * 
 * @note Ctlr / can be ignored with SIGIGN
 * @note Ctlr D sends a NULL to readline or EOF to heredoc
 */
int	main(int argc, char **argv, char **envp)
{
	t_args	*head;

	head = NULL;
	if (argc < 0)
		argv[argc] = NULL;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_handler);
	minishell(head, envp);
	return (EXIT_SUCCESS);
}
