





/**
 * @brief Counts the occurence of c
 * 
 * @param c the character to count
 * 
 * @note c between quotes is not considered
*/
int	cnt_occur_skip1(char *input, char c)
{
	int		i;
	int		cnt;

	i = 0;
	cnt = 0;
	
	while(input[i])
	{
		if (incl_char(input[i], "\'\""))
			i += skip_over(input, input[i], i + 1);
		if (input[i] == '|')
			cnt++;
		i++;
	}
	return (cnt);
}




char *cut_tok(char *input, int start, int flag)
{
	char *res;
	int	len;

	len = start;
	while (input[len] && !incl_char(input[len], " >|<"))
		len++;
	len -= start;
	res = ft_substr(input, start, len);
	input = del_substr(input, start, len);
	if (!res || !input)
		return (NULL);
	return (res);
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

char *prep_tok(char *input)
{
	int i;

	i = 0;
	while (input[i])
	{
		if (incl_char(input[i], ">|<"))
			return (cut_tok(input, i, OPER));
		if (ft_isalnum(input[i]) || incl_char(input[i], "$"))
			return (cut_tok(input, i, ARG));
		if (incl_char(input[i], "\'\""))
			return (cut_tok(input, i, QUOTE));
		i++; // Skip spaces
	}
	return (NULL);
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



/**
 * @brief sth here
 * 
 * @param len The length of the string. If len is -1 then str is NULL
*/
char	*prep_cmd(char *str, int len)
{
	char *temp;
	int	i;
	int	k;

	i = 1;
	if (len != -1 && str[0] == '|')
	{
		str[0] = '/';
		while (str[i] && str[i] == ' ')
			i++;
		str = del_substr(str, 1, i - 1);
		return (str);
	}
	k = 0;
	temp = malloc(sizeof(char ) * (len + 2));
	if (!temp || len == -1)
		return (NULL);
	temp[len + 1] = '\0';
	temp[0] = '/';
	
	while (str[k] && i < len + 1)
		temp[i++] = str[k++];
	return (temp);
}


// Improvement Ideas
	// 1. strlcpy temp & up to $
	// 2. If !env_var skip over while loop and just copy from there
char	*substitute_var(char *str, char *env_var, int env_len, t_data *data)
{
	char				*temp;
	int					i;
	int					k;
	int					temp_i;

	i = 0;
	k = 0;
	if (!env_var)
		return (del_substr(str, data->env_start - 1, data->env_len + 1));
	temp = malloc(sizeof(char) * (data->env_size + 1));
	if (!temp || !str)
		return (NULL);
	temp[data->env_size] = '\0';
	i += copy_before(&temp, str, data); // Use strlcpy here
	temp_i = env_len + i; 
	while (env_var[k]) // Only copy if env_var exists if not just skip
		temp[i++] = env_var[k++];
	while (str[temp_i])
		temp[i++] = str[temp_i++];
	free(str);
	return (temp);
}