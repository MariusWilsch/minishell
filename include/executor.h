/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 11:18:31 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/24 13:08:09 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	char *cmd; // The resoluted cmd
	// enum token for cmds
	char	**args;			//	The names of the arguments for the command
	t_operation *oper;
} t_cmds;


