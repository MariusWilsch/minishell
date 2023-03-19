/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 10:37:57 by verdant           #+#    #+#             */
/*   Updated: 2023/03/19 17:03:32 by mwilsch          ###   ########.fr       */
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
		ft_printf("%d:\t", temp->type);
		ft_printf("`%s`\n\n", temp->arg);
		temp = temp->next;
	}
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
		free(head);
		head = head->next;
	}
	// atexit(leaks);
}

// Does readline cause the leak, like the subject says?


/**
 * @note Do I need the quotes or should I leave them already?
 * 
 * @note 
 * I should spent an hour or 2 on testing inputs
 * ToDo adding redirect checking logic
 * Do I want that free gets freed in sub_var
*/
int	main(void)
{
	char	*input = ft_strtrim(readline(""), " ");  // Reading the cmd line input
	// char	*input = ft_strtrim("echo $HOE and some text", " "); // When Debugging

	t_args *head;
	if (!input || !are_quotes_even(input))
		return (debug_msg("main: input or quotes\n"));
	head = create_tok_list(input, head);
	head = process_tok(head);
	// print_list(head);
	// free_list(head);
	return (free(input), EXIT_SUCCESS);
}

