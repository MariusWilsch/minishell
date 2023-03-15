/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 11:52:23 by verdant           #+#    #+#             */
/*   Updated: 2023/03/15 16:50:08 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing_v2.h"

t_cmd *structs_init(char *input, t_cmd *cmds, t_data *data)
{	
	if (input[0] == '\0') // I have to change this to reprompt
		return (NULL);
	data->delim = " <>";
	data->skip = "\'\"";
	data->env_len = 0;
	data->env_size = 0;
	data->cmd_cnt = 1;
	data->cmd_cnt += count_occurences_skip(input, '|', NULL);
	data->redir_cnt = 0;
	cmds = malloc(sizeof(t_cmd) * (data->cmd_cnt));
	if (!cmds)
		return (NULL);
	return (cmds);
}

/**
 * @brief  Main
 * 
 * @note I think the things which are left do are
 * 0  (Decide to implement or not implement my refactor ideas)
 * 1. Executing the redirection && where to create the fds
 * 2. Adding a rule for the built-in functions
 * 3. Display a prompt when waiting for a new command.
 * 4. Have a working history.
 * 5. Execution of pipes
 * 6. Signals
 * 
*/

int main(int argc, char *argv[])
{
	char				*input = readline(""); // Reading the cmd line input
	// char				*input = "ls >> test < test2"; // Use when DEBUG=1
	char				**arr;
	t_cmd				*cmds;
	t_data			data;
	int					i;
	
	i = 0;
	cmds = structs_init(input, cmds, &data);
	if (!cmds || !are_quotes_even(input)) 
		return (1);
	arr = ft_split(input, '|'); // Check for more than 1 pipes
	// Check if splitting was succesfull // Do this later
	while (i < data.cmd_cnt)
	{
		if (!cmd_res(arr[i], &cmds[i], &data))
			return (1);
		// if (!redirect_pars(arr[i], &data))
		// 	return (1);
		if (!env_res(&arr[i], &data))
			return (1);
		// ft_printf("CMDS:\t|%s|\n", cmds[i].name);
		// ft_printf("ARGS:\t|%s|\n\n", arr[i]);
		i++;
	}
}

// Tester main
// int main(int argc, char *argv[])
// {
// 	char				*input = readline(""); // Reading the cmd line input
// 	// char				*input = "ls <     >"; // Use when DEBUG=1
// 	char				**arr;
// 	t_cmd				*cmds;
// 	t_data			data;
// 	int					i;
	
// 	i = 0;
// 	cmds = structs_init(input, cmds, &data);
// 	if (!cmds || !are_quotes_even(input)) 
// 		return (1);
// 	arr = ft_split(input, '|');
// 	// Check if splitting was succesfull // Do this later
// 	while (i < data.cmd_cnt)
// 	{
// 		if (!cmd_res(arr[i], &cmds[i], &data)) //  || 
// 			return (1);
// 		if (!env_res(&arr[i], &data))
// 			return (1);
// 		// if (!redirect_pars(arr[i], &data))
// 		// 	return (1);
// 		printf("CMDS:\t|%s|\n", cmds[i].name);
// 		printf("ARGS:\t|%s|\n\n", arr[i]);
// 		i++;
// 	}
// }