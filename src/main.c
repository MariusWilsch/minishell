/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 10:37:57 by verdant           #+#    #+#             */
/*   Updated: 2023/03/17 13:06:05 by mwilsch          ###   ########.fr       */
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



int	get_env_len(char *str)
{
	const	int start = ft_strclen(str, '$') + 1;
	int	len;

	len = start;
	while (str[len] && !incl_char(str[len], " <>\""))
		len++;
	return (len - start);
}


/**
 * @brief prepears the string for getenv
 * 
 * @note 
 * 
 * @return Returns the length of string with the subsituted env_var
 * but without the $HOME for example - Edit this
*/
int	get_env(char *str,char **env_var, int len, t_type_tok type)
{
	const int start = ft_strclen(str, '$') + 1;
	char *temp;

	temp = ft_substr(str, start, len);
	if (!temp)
		return (-1);
	*env_var = getenv(temp);
	free(temp);
	len = (ft_strlen(str) + ft_strlen(*env_var)) - len;
	return (len - 1);
}

char	*env_res(char *str, int env_len, t_type_tok type)
{
	char			*env_var;
	char			*new;
	int				len;

	len = get_env(str, &env_var, env_len, type);
	new = malloc(sizeof(char) * (len + 1));
	if (!new || len == -1)
		return (NULL);
	new[len] = '\0';
	len = ft_strlcpy(new, str, ft_strclen(str, '$') + 1);
	len = ft_strlcat(new, env_var, len + ft_strlen(env_var) + 1);
	env_len += ft_strclen(str, '$') + 1;
	while (str[env_len] && env_len < len)
		new[len++] = str[env_len++];
	ft_printf("|%s|\n", new);
	return (NULL);
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
		{
			env_res(temp->arg, get_env_len(temp->arg), temp->type);
		}
		temp = temp->next;
	}
	// print_list(head);
	return ;
}

t_args	*tokenizer(char *input, t_args *head)
{
	head = create_tok_list(input, head);
	// print_list(head);
	process_tok(head);
	return (head);
}

void	leaks(void)
{
	system("leaks -q minishell");
}

int	main(int argc, char *argv[])
{
	// With strtrim it will proab. cause mem leak
	char	*input = ft_strtrim(readline(""), " ");  // Reading the cmd line input
	// char	*input = ft_strtrim("echo    |    ls", " "); // When Debugging
	

	
	t_args *head;

	if (!input || !are_quotes_even(input))
		return (debug_msg("main: input or quotes\n"));
	head = tokenizer(input, head);
	
	
	// while (head != NULL) // Freeing the list
	// {
	// 	free(head->arg);
	// 	head = head->next;
	// }
	// atexit(leaks);
	free(head);
	return (free(input), EXIT_SUCCESS);
}

