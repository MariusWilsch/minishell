bool	redir_init(t_redir *redir, char *str, t_err_tok err_type, int redirc)
{
	const char	c = str[0];
	const int		cnt = cnt_occur(str, str[0]);
	int					file_length;
	// t_redirect_type type[4] = [TRUNC, APPEND, INPUT, INPUT_EOF];

	file_length = ft_strlen(str) - cnt;
	if (err_type == ENV_REDIRECT_ERR)
		return (redir->type = ERR, true);
	redir->filename = ft_substr(str, cnt, file_length);
	if (!redir->filename)
		return (false);
	if (c == '>' && cnt == 1)
		redir->type = TRUNC;
	if (c == '>' && cnt == 2)
		redir->type = APPEND;
	if (c == '<' && cnt == 1)
		redir->type = INPUT;
	if (c == '<' && cnt == 2)
		redir->type = INPUT_EOF;
	redir->redirc = redirc;
	return (true);
}


void	init_members(t_cmds *cmd, t_args *head, int redirc)
{
	cmd->cmd_path = head->arg;
	cmd->cmd_type = CMD_EXE;
	if (head->type == BUILT_IN && cmd->cmd_type == CMD_EXE)
		cmd->cmd_type = BUILT_IN_EXE;
	if (incl_char(head->arg[0], "<>"))
	{
		cmd->cmd_path = NULL;
		cmd->cmd_type = NO_CMD_EXE;
	}
	cmd->redir = NULL;
	cmd->argv = ft_calloc(sizeof(char *), (cmd->argc + 1));
	if (redirc > 0)
		cmd->redir = ft_calloc(sizeof(t_redir), redirc);
	if (!cmd->argv || (redirc > 0 && !cmd->redir))
		return ;
	cmd->argv[0] = head->arg;
}

t_args	*fill_struct(t_cmds *cmd, t_args *head, int argc, int redirc)
{
	int	i;
	int	k;

	i = 1;
	k = 0;
	cmd->argc = argc;
	init_members(cmd, head, redirc);
	if (cmd->argc == 0 && redirc == 0)
		return (head->next);
	while (true)
	{
		if (i < cmd->argc && (head->type == ARG || head->type == QUOTE_ARG))
			cmd->argv[i++] = head->arg;
		if (k < redirc && head->type == REDIR)
			redir_init(&cmd->redir[k++], head->arg, head->err_tok, redirc);
		head = head->next;
		if (i == cmd->argc && k == redirc)
			break ;
	}
	return (head);
}

/**
 * @note ARGUMENTS ARE BEING COUNTED. 
*/
static void	arg_counter(t_args *node, t_cmds *cmd, t_args *head, int cmd_cnt)
{
	int	i;
	int	redir_i;
	int	arg_cnt;

	i = 0;
	arg_cnt = 0;
	if (node->type == CMD || node->type == BUILT_IN)
		node = node->next;
	while (i < cmd_cnt)
	{
		arg_cnt = 1;
		redir_i = 0;
		while (node != NULL && node->type != CMD && node->type != BUILT_IN)
		{
			if ((node->type == ARG || node->type == QUOTE_ARG))
				arg_cnt++;
			if (node->type == REDIR)
				redir_i++;
			node = node->next;
		}
		head = fill_struct(&cmd[i++], head, arg_cnt, redir_i);
		if (node != NULL)
			node = node->next;
	}
}

/**
 * 
 * @note What if there there is no cmd but a redirect as the first string
 * i = number of commmands.
 * 
*/
t_cmds	*create_structs(t_args *head, int *cmd_cnt, t_env **env)
{
	t_args		*node;
	t_cmds		*cmds;
	int			cmd_count;

	node = head;
	cmd_count = 0;
	if (head->type == REDIR)
			cmd_count++;
	while (node != NULL)
	{
		if (node->type == CMD || node->type == BUILT_IN)
			cmd_count++;
		node = node->next;
	}
	cmds = ft_calloc(sizeof(t_cmds), cmd_count);
	if (!cmds)
		return (NULL);
	node = head;
	arg_counter(node, cmds, head, cmd_count);
	*cmd_cnt = cmd_count;
	if (head->type == REDIR)
		cmd_cnt--;
	while (cmd_count--)
		cmds[cmd_count].env = env;
	return (cmds);
}
