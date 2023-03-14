/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 10:37:57 by verdant           #+#    #+#             */
/*   Updated: 2023/03/13 13:29:43 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	debug_msg(char *str)
{
	ft_printf("%s", str);
	return (1);
}

/**
 * @brief Intialise structs and important data
 * 
 * @param str command line input
 * @param cmds command table struct
 * @param data data struct

 */
t_cmds	*struct_init(char *str, t_cmds *cmds, t_data *data)
{
	const int	cnt_pipe = cnt_occur_skip(str, '|', "\'\"");
	
	data->cmd_cnt = 1;
	ft_printf("%d\n", data->cmd_cnt + cnt_pipe);
	cmds = malloc(sizeof(t_cmds) * (data->cmd_cnt + cnt_pipe));
	if (!cmds)
		return (NULL);
	return (cmds);
}

void	print_2D_arr(char **arr) // Delete after completion
{
	if (!arr)
		return ;
	for (int i = 0; arr[i]; i++)
		ft_printf("%s\n", arr[i]);
	return ;
}

char **prep_for_parser(char *input, t_cmds **cmds, t_data *data)
{
	char **arr = NULL; // Delete = NULL later
	

	
	if (!input || !check_input(input))
	{
		puts("In here");
		return (NULL);
	}
	// return (NULL); // For debugging purposes
	*cmds = struct_init(input, *cmds, data);
	if (!(*cmds) || !data)
		return (NULL);
	

	
	// If a pipe is between quotations then ft_split has to skip that pipe
		// my ft_split_ultimate should fix that problem
	





	// (*cmds)->cmd_name = "TEST";
	// printf("%s\n", (*cmds)->cmd_name);
	// print_2D_arr(arr); // Just for debugging purposes
	return (arr);
}


/**
 * @brief main function
 * 
 * @return int 
 */
int	main(void)
{
	t_cmds	*cmds;
	t_data	data;
	char		*input;
	char		**arr;

	while (1)
	{
		input = readline("Minishell-0.1$ ");
		// input = "echo test";
		// arr = prep_for_parser(input, &cmds, &data);
		printf("%d", check_input(input));
		free(input);
		break ; //  For debugging purposes
	}
	return (EXIT_SUCCESS);
}

// int	main(void)
// {
// 	t_cmds	*cmds;
// 	t_data	data;
// 	char		*input;
// 	char		**arr;

// 	while (1)
// 	{
// 		input = readline("Minishell-0.1$ ");
// 		if (!input || !check_input(input))
// 			return (debug_msg("main: !input or check_input"));
// 		arr = ft_split(input, '|');
// 		cmds = struct_init(input, cmds, &data);
// 		print_2D_arr(arr);
// 		if (!arr || !cmds)
// 			return (debug_msg("main: ft_split or struct_init"));
// 		free(input);
// 	}
// 	return (EXIT_SUCCESS);
// }


