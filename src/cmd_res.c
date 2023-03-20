/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_res.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 19:32:28 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/20 15:23:58 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


bool	is_builtin(t_args *node)
{
	if (ft_strncmp("echo", node->arg, ft_strlen(node->arg)) == 0) // just add nums instead of strlen
		return (node->type = BUILT_IN, true);
	if (ft_strncmp("pwd", node->arg, ft_strlen(node->arg)) == 0)
		return (node->type = BUILT_IN, true);
	if (ft_strncmp("cd", node->arg, ft_strlen(node->arg)) == 0)
		return (node->type = BUILT_IN, true);
	if (ft_strncmp("exit", node->arg, ft_strlen(node->arg)) == 0)
		return (node->type = BUILT_IN, true);
	if (ft_strncmp("env", node->arg, ft_strlen(node->arg)) == 0)
		return (node->type = BUILT_IN, true);
	if (ft_strncmp("unset", node->arg, ft_strlen(node->arg)) == 0)
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
	char *str = node->arg;
	if (ft_strchr(str, '$'))
	{
		str = sub_env(str, get_env_len(str)); // Leak? 
		if (!ft_strchr(str, '/'))
			return (resolute_cmd(node, ft_strdup(node->arg)));
		ft_printf("minishell: %s: No such file or directory\n", str);
		node->err_tok = NO_FILE_DIR;
		return (str);
	}
	ft_printf("minishell: %s: command not found\n", del_quotes(str));
	node->err_tok = NO_CMD;
	return (str);
}


/**
 * @brief Preparing the command to add it to the path directory
*/
char *prep_cmd(char *str)
{
	char *temp;
	int	i;

	i = 1;
	if (str[0] == '|')
	{
		while (str[i] && str[i] == ' ')
			i++;
		str = del_substr(str, 0, i);
	}
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
	if (cmd == NULL)
		return (free(cmd), cmd_err(node));
	return (free(node->arg), cmd);
}
