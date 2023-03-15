/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 10:37:57 by verdant           #+#    #+#             */
/*   Updated: 2023/03/15 18:57:13 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	debug_msg(char *str)
{
	ft_printf("%s", str);
	return (1);
}


void print_list(t_args *head)
{
	t_args *temp = head;
	// ft_printf("%s", temp->);
	while (temp != NULL)
	{
		ft_printf("%d:\t", temp->type);
		ft_printf("`%s`\n", temp->arg);
		temp = temp->next;
	}
}


void	env_res(char *str, int start, t_type_tok type)
{
	char *env_var;
	char *temp;
	int		len;

	len = start;
	temp = NULL;
	if (type == QUOTE_ARG && str[0] == '\"')
	{
		while (str[len] && !incl_char(str[len], " <>\""))
			len++;
		len = len - start;
		temp = ft_substr(str, start, len);
	}
	ft_printf("%s ", str);
	if (start == 0 && type == ARG)
		temp = del_substr(str, 0, 1);
	ft_printf("%s", temp);
	// env_var = getenv(temp);
	// if (type == QUOTE_ARG)
		free(temp);
	// return (temp);
}



void	process_tok(t_args *head)
{

	t_args *temp = head;

	while (temp != NULL)
	{
		if (temp->type == CMD)
			temp->arg = resolute_cmd(temp->arg);
		// if (temp->type == REDIRECT)
			// Do red_checks
		if (ft_strchr(temp->arg, '$') && temp->arg[0] != '\'')
			env_res(temp->arg, ft_strclen(temp->arg, '$') + 1, temp->type);
		temp = temp->next;
	}
	// print_list(head);
	return ;
}

void	tokenizer(char *input, t_args *head)
{
	head = create_tok_list(input, head);
	// print_list(head);
	process_tok(head);
}


int	main(int argc, char *argv[])
{
	// With strtrim it will proab. cause mem leak
	char	*input = ft_strtrim(readline(""), " ");  // Reading the cmd line input
	// char	*input = ft_strtrim("echo    |    ls", " "); // When Debugging
	

	
	t_args *head;

	if (!input || !are_quotes_even(input))
		return (debug_msg("main: input or quotes\n"));
	tokenizer(input, head);
	return (EXIT_SUCCESS);
}

