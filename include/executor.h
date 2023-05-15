/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: verdant <verdant@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/24 11:18:31 by mwilsch       #+#    #+#                 */
/*   Updated: 2023/05/15 15:01:13 by tklouwer      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

int						g_status;

typedef struct t_args	t_args;
typedef struct s_env	t_env;

typedef enum e_cmd_type {
	CMD_EXE,
	BUILT_IN_EXE,
	NO_CMD_EXE,
}	t_cmd_type;

typedef enum e_redirect_type {
	TRUNC,
	APPEND,
	INPUT,
	INPUT_EOF,
	ERR,
}	t_redirect_type;

typedef struct s_redir {
	char			*redirect;
	char			*filename;
	t_redirect_type	type;
	int				redirc;
	struct t_redir	*next;
}	t_redir;

typedef struct s_cmd {
	char		*cmd_path;
	char		**argv;
	int			argc;
	int			in_fd;
	int			out_fd;
	int			status;
	t_env		**env;
	t_cmd_type	cmd_type;
	t_redir		*redir;
}	t_cmds;

/* 			INIT_STRUCTS		 */
t_cmds	*create_structs(t_args *head, int *cmd_cnt, t_env **env);

/* 			EXECUTOR			 */
int		executor(t_args *head, t_env **env);

/* 			BUILT-INS			 */
int		echo(int argc, char **argv);
int		cd(int argc, char *path, t_env *env_list);
int		pwd(void);
int		env(t_env **env_list);
int		export(int argc, char *argv[], t_env **env_list);
int		unset(int argc, char *argv[], t_env **env_list);
int		exec_builtin(char *func, int argc, char **argv, t_env **env_list);
int		exisit_env(t_env **env_list, char *str, t_env **found);

/* 				UTILS			 */
void	execute_command(t_cmds *head);
int		p_error(char *str, int status);
int		wr_dup2(int fd1, int fd2);
void	cleanup(int cmd_cnt, t_cmds *cmd);

/* 				CHILD PROCESS	 */
int		child_process(t_cmds *cmd, int i, int cmd_cnt, int *pipe_fd);

/* 				REDIR IO		 */
int		heredoc(const char *delimiter);
int		handle_redirects(t_cmds *head);
int		redirect_input(t_redir *redir);
int		redirect_output(t_redir *redir);

#endif