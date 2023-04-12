/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 11:18:31 by mwilsch           #+#    #+#             */
/*   Updated: 2023/04/12 13:06:18 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct t_args t_args;

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

typedef struct s_redir {
	char			*redirect;
	char			*filename;
	t_redirect_type	type;
} t_redir;


typedef struct s_cmd {
	char		*cmd_path;
	char		**argv;
	int			argc;
	// int			rd;
	t_cmd_type	cmd_type;
	t_redir		*redir;
} t_cmds;


/* 			INIT_STRUCTS		 */

t_cmds	*create_structs(t_args *head, int *cmd_cnt);

/* 			EXECUTOR			 */
int 	executor(t_args *head);

/* 			BUILT-INS			 */

int echo(int argc, char **argv);
int cd(int argc, char *path);
int pwd(void);
int sh_exit(int status);
int env();
int unset(int argc, char *argv[]);
int exec_builtin(char *func, int argc, char **argv);

/* 				UTILS			 */

void	execute_command(t_cmds *head);
int		p_error(char *str, int status);
int		wr_dup2(int fd1, int fd2);

/* 				REDIR IO		 */

int handle_redirects(t_cmds *head, int *end);
int redirect_input(t_cmds *head, int *end);
int	redirect_output(t_cmds *head, int *end);