/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 11:18:31 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/24 15:35:34 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef enum e_cmd_type {
	CMD_EXE,
	BUILT_IN_EXE,
	NO_CMD_EXE,
} t_cmd_type;

typedef enum e_redirect_type {
	TRUNC,
	APPEND,
	INPUT,
	INPUT_EOF,
	ERR,
} t_redirect_type;



typedef struct s_operation {
	char						*redirect;
	char						*filename;
	t_redirect_type	type;
} t_operation;


typedef struct s_cmd {
	char *cmd_path; // The resoluted cmd
	char	**args;			//	The names of the arguments for the command
	t_cmd_type cmd_type;
	t_operation *oper;
} t_cmds;


