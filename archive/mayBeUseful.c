


int	get_env(char *str,char **env_var, t_type_tok type)
{
	const int start = ft_strclen(str, '$') + 1;
	char *temp;
	int		len;

	len = ft_strlen(str) - 1;
	if (type == QUOTE_ARG && str[0] == '\"')
	{
		len = start;
		while (str[len] && !incl_char(str[len], " <>\""))
			len++;
		len = len - start;
	}
	ft_printf("%d\n", len);
	temp = ft_substr(str, start, len);
	if (!temp)
		return (-1);
	*env_var = getenv(temp);
	free(temp);
	len = (ft_strlen(str) + ft_strlen(*env_var)) - len;
	return (len - 1);
}


/**
 * @brief Counts occurences of char c inside of a string
 * 
 * @param str String to search occurence in
 * @param c the character to search for
 * 
 * @note Does not count the occurence if the char c is inside of 
 * quotations
 */
int	cnt_occur_skip(char *str, char c, char *skip)
{
	int	i;
	int	cnt;
	
	i = 0;
	cnt = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (incl_char(str[i], skip)) // Could be it's own int function i += skip()
		{
			i++;
			while (!incl_char(str[i], skip) && str[i + 1] != '\0')
				i++;
		}
		if (str[i] == c)
			cnt++;
		i++;
	}
	return (cnt);
}


void	get_tok(char *input)
{
	char *tok;
	int i;

	i = 0;
	while (input[i])
	{
		if (incl_char(input[i], ">|<"))
		{
			tok = cut_tok_operator(input, i);
			printf("1: %s\n", tok);
			i = 0;
		}
		if (ft_isalnum(input[i]) || incl_char(input[i], "\'\"$"))
		{
			tok = cut_tok(input, i);
			printf("2: %s\n", tok);
			i = 0;
		}
		i++; // Skip spaces
	}
}



char	*resolute_cmd(char *str)
{
	const char *path = getenv("PATH");
	char *temp;
	char **path_2D;
	int	i;

	i = 0;
	str = prep_cmd(str); // Malloc
	if (!str || !path)
		return (NULL);
	path_2D = ft_split(path, ':');  // Malloc
	while (path_2D[i] != NULL)
	{
		temp = ft_strjoin(path_2D[i], str); // Malloc Loop
		if (!temp)
			return (free_split(path_2D), NULL);
		if (access(temp, X_OK) == 0)
			break ;
		free(temp);
		i++;
	}
	if (path_2D[i] == NULL)
		return (what_went_wrong_bruh(str)); 
	free(str);
	free_split(path_2D);
	return (temp);
}






test $HOE test

char	*get_tok(char *input, int start, t_type_tok type)
{
	char	*res;
	int		len;

	len = start;
	if (type != QUOTE_ARG)
	{
		while (type == OPERATOR && input[len] && incl_char(input[len], " >|<"))
			len++;
		while (input[len] && !incl_char(input[len], " >|<"))
		{
			if (incl_char(input[len], "\'\""))
				len += cnt_len_between(input, input[len], len + 1) + 1;
			len++;
		}
		len -= start;
	}
	if (type == QUOTE_ARG)
		len = cnt_len_between(input, input[start], start + 1) + 2;
	res = ft_substr(input, start, len);
	input = del_substr(input, start, len);
	if (!res || !input)
		return (NULL);
	return (res);
}

int	add_tok(char *str, t_args **head, t_type_tok type)
{
	t_args	*new;
	t_args	*temp;

	temp = *head;
	new = malloc(sizeof(t_args));
	if (!new)
		return (-1);
	new->arg = str;
	new->type = type;
	new->err_tok = OK;
	new->next = NULL;
	if (str[0] == '|' && type == OPERATOR)
		new->type = CMD;
	if (incl_char(str[0], "><") && type == OPERATOR)
		new->type = REDIRECT;
	if (!temp) // This marks the first node
	{
		*head = new;
		if (type == ARG)
			new->type = CMD;
		new->next = NULL;
		return (0);
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	return (0);
}
 

 	// this is for 
		// 1. bash: /Users/mwilsch: Is a directory (when >$HOME)
		// 2. bash: /Users/mwilsch test: Permission denied (when for example >"$HOME test")
	// but this is frankenstein code and I need refactor this or think what could be done better
	if (!ft_strchr(str, '\'') && ft_strchr(str, '$'))
	{
		str = sub_env(ft_strdup(str), get_env_len(str));
		if (!ft_strchr(str, '\"'))
		{
			str = del_substr(str, 0, cnt);
			if (access(str, F_OK) == 0)
				ft_printf("%s: Is a directory", str);
			return (1);
		}
		str = ft_substr(str, ft_strclen(str, '\"') + 1, cnt_len_between(str, '\"', ft_strclen(str, '\"') + 1));
		if (access(str, R_OK) == -1)
			ft_printf("%s: Permission Denied", str);
		return (1);
	}

	int	env_var_case(char *str, char *dup, int len)
{
	// const int start = ft_strclen(str, '\"');
	
	while (dup[len] && dup[len] == ' ')
		len++;
	dup = del_substr(dup, 0, len);
	str = sub_env(dup, get_env_len(dup));
	if (str[0] != '\"')
	{
		if (access(str, F_OK) == 0)
			return (ft_printf("minishell: %s: Is a directory", str));
	}
	str = del_quotes(str);
	ft_printf("%s", str);
	if (access(str, W_OK) == 0)
		ft_printf("minishell: %s: Permission Denied", str);
	return (2);
}