/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 10:37:57 by verdant           #+#    #+#             */
/*   Updated: 2023/03/17 17:38:50 by mwilsch          ###   ########.fr       */
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



t_args	*process_tok(t_args *head)
{
	t_args *node = head;

	while (node != NULL)
	{
		if (node->type == CMD)
			node->arg = resolute_cmd(node, ft_strdup(node->arg));

		// ft_printf("%s", node->arg);


		// if (node->type == REDIRECT)
			// Do red_checks
		// if (ft_strchr(node->arg, '$') && node->arg[0] != '\'')
		// {
		// 	while (ft_strchr(node->arg, '$'))
		// 		node->arg = env_res(node->arg, get_env_len(node->arg));
		// }
		node = node->next;
	}
	// print_list(head);
	return (head);
}

t_args	*tokenizer(char *input, t_args *head)
{
	head = create_tok_list(input, head);
	head = process_tok(head);
	return (head);
}




// Does readline cause the leak, like the subject says?
int	main(void)
{
	char	*input = ft_strtrim(readline(""), " ");  // Reading the cmd line input
	// char	*input = ft_strtrim("echo$HOME", " "); // When Debugging

	t_args *head;
	if (!input || !are_quotes_even(input))
		return (debug_msg("main: input or quotes\n"));
	head = tokenizer(input, head);
	
	print_list(head);
	// while (head != NULL) // Freeing the list
	// {
	// 	free(head->arg);
	// 	free(head);
	// 	head = head->next;
	// }
	// atexit(leaks);
	return (free(input), EXIT_SUCCESS);
}

