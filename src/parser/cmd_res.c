/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_res.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 19:32:28 by mwilsch           #+#    #+#             */
/*   Updated: 2023/04/03 14:19:04 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


bool	is_builtin(t_args *node)
{
	if (ft_strncmp("echo", node->arg, 4) == 0) // just add nums instead of strlen
		return (node->type = BUILT_IN, true);
	if (ft_strncmp("pwd", node->arg, 3) == 0)
		return (node->type = BUILT_IN, true);
	if (ft_strncmp("cd", node->arg, 2) == 0)
		return (node->type = BUILT_IN, true);
	if (ft_strncmp("exit", node->arg, 4) == 0)
		return (node->type = BUILT_IN, true);
	if (ft_strncmp("env", node->arg, 3) == 0)
		return (node->type = BUILT_IN, true);
	if (ft_strncmp("unset", node->arg, 5) == 0)
		return (node->type = BUILT_IN, true);
	return (false);
}

/**
 * @brief Writes error messages
 * 
 * @param node the node of the linked list where a error occured
 */
char	*cmd_err(t_args *node)
{
	int start;
	char *str;

	str = node->arg;
	if (str[0] == '.')
	{
		ft_printf("minishell: %s: No such file or Directory\n", str);
		node->err_tok = NO_FILE_DIR;
		return (str);
	}
	if (ft_strchr(str, '$'))
	{
		str = sub_env(str, get_env_len(str)); // Leak? 
		if (!ft_strchr(str, '/'))
			return (resolute_cmd(node, ft_strdup(node->arg)));
		ft_printf("minishell: %s: No such file or directory\n", str);
		return (node->err_tok = NO_FILE_DIR, str);
	}
	ft_printf("minishell: %s: command not found\n", del_quotes(str));
	if (node->arg[0] == '|')
		node->arg = del_substr(node->arg, 0, cnt_occur(node->arg + 1, ' ') + 1);
	start = cnt_occur(node->arg, node->arg[0]);
	if (incl_char(node->arg[0], "><"))
		node->arg = del_substr(node->arg, start, cnt_occur(node->arg + start, ' '));
	return (node->err_tok = NO_CMD, str);
}


/**
 * @brief Preparing the command to add it to the path directory
*/
char *prep_cmd(char *str)
{
	char *temp;
	int	i;

	i = 1;
	temp = NULL;
	if (str[0] == '|')
	{
		while (str[i] && str[i] == ' ')
			i++;
		str = del_substr(str, 0, i);
	}
	if (str[0] == '.')
		return (temp);
	temp = ft_strjoin("/", str);
	free(str);
	if (!temp)
		return (NULL);
	return (temp);
}



/**
 * @brief Searching for the executable using the PATH variable
 * 
 * @param node A single node of the linked list containg a string to be resoluted
 * @param cmd A duplicate of the orginal string
 * @return cmd If command resolution was succesfull and cmd_err which returns the unchanged string
 * 
 * @note I need to also resolute in the current working directory
 */
char	*resolute_cmd(t_args *node, char *cmd)
{
	char *temp;
	char **path_2D;
	int	i;

	i = 0;
	if (access(cmd, X_OK) == 0)
		return (free(node->arg), cmd);
	temp = prep_cmd(cmd); // Malloc
	path_2D = ft_split(getenv("PATH"), ':');  // Malloc
	while (path_2D && temp && path_2D[i] != NULL)
	{
		cmd = ft_strjoin(path_2D[i], temp); // Malloc Loop
		if (!cmd)
			return (free_split(path_2D), cmd);
		if (access(cmd, X_OK) == 0)
			break ;
		i++;
		free(cmd);
		cmd = NULL;
	}
	free(temp);
	free_split(path_2D);
	if (cmd == NULL || temp == NULL)
		return (free(cmd),cmd_err(node));
	return (free(node->arg), cmd);
}
