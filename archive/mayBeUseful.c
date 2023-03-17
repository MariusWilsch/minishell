


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