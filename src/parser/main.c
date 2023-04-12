/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 10:37:57 by verdant           #+#    #+#             */
/*   Updated: 2023/04/12 15:23:41 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <sys/stat.h>


// Delete when finished
int	debug_msg(char *str)
{
	ft_printf("%s", str);
	return (1);
}

// Delete when finished
void print_list(t_args *head)
{
	t_args *temp = head;
	while (temp != NULL)
	{
		ft_printf("ARG TYPE: %d\t", temp->type);
		ft_printf("ERR TOK: %d\n", temp->err_tok);
		ft_printf("`%s`\n\n", temp->arg);
		temp = temp->next;
	}
	ft_printf("-----------------------------\n");
}

// Delete when finished
void	leaks(void)
{
	system("leaks -q minishell");
}

void	free_list(t_args *head)
{
	while (head != NULL) // Freeing the list
	{
		free(head->arg);
		head = head->next;
	}
	free(head);
	// atexit(leaks);
}

// Does readline cause the leak, like the subject says?
char *prompt(char *str)
{
	char *input;
	
	input = NULL;
	if (!str)
		return (NULL);
	if (!str[0])
		return (str);
	input = ft_strtrim(str, " ");
	free(str);
	if (!input)
		return (NULL);
	if (input[0] != '\0')
		add_history(input);
	return (input);
}

/**
 * @note Do I need the quotes or should I leave them already?
 * 
 * @note 
 * I should spent an hour or 2 on testing inputs
 * Do I want that free gets freed in sub_var

*/
int minishell(t_args *head)
{
	char	*input;
	
	while (1)
	{
		head = NULL;
		input = prompt(readline("Minishell-1.0$ "));
		if (!input)
			p_error("prompt failed", 1);
		if (!input[0] || !are_quotes_even(input))
		{
			free(input);
			continue ;
		}
		head = create_tok_list(input, head);
		head = process_tok(head);
		if (head->type == REPROMPT)
		{
			free_list(head);
			continue ;
		}
		executor(head);
		free_list(head);
		free(input);
		if (freopen("/dev/tty", "r", stdin) == NULL) { // m1 disassoicates stdin from terminal
				perror("freopen");
				return EXIT_FAILURE;
		}
	}
}
int	main(void)
{
	t_args *head;

	minishell(head);
	return (EXIT_SUCCESS);
}






