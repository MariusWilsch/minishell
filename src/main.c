/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwilsch <mwilsch@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/10 10:37:57 by verdant       #+#    #+#                 */
/*   Updated: 2023/03/24 13:20:02 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (!str[0])
		return (str);
	if (!str)
		return (NULL);
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
// int	main(void)
// {
// 	char		*input;
// 	t_args	*head;

// 	// while (1)
// 	// {
// 	// 	head = NULL;
// 		// input = prompt(readline("Minishell-1.0$ "));
// 		input = ft_strtrim(">>test echo | >> test ls", " ");
// 		if (!input) 
// 			return (EXIT_FAILURE);
// 		if (!input[0] || !are_quotes_even(input))
// 		{
// 			free(input);
// 			// continue ;
// 		}
// 		head = create_tok_list(input, head);
// 		head = process_tok(head);
// 		// if (head->type == REPROMPT)
// 		// {
// 		// 	free_list(head);
// 		// 	// continue ;
// 		// }
// 		print_list(head);
// 		// executor(head);
// 		// free_list(head);
// 		free(input);
// 	// }
// 	// rl_clear_history()
// 	return (EXIT_SUCCESS);
// }

int	main(void)
{
	char	*input;
	t_args	*head;

	while (1)
	{
		head = NULL;
		input = prompt(readline("Minishell-1.0$ "));
		if (!input) 
			return (EXIT_FAILURE);
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
		// print_list(head);
		executor(head);
		free_list(head);
		free(input);
	}
	// rl_clear_history()
	return (EXIT_SUCCESS);
}






