





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

char	*cut_tok_operator(char *input, int start)
{
	char	*res;
	int		len;

	len = start;
	while (input[len] && incl_char(input[len], " ><"))
		len++;
	while (input[len] && !incl_char(input[len], " ><"))
		len++;
	len -= start;
	res = ft_substr(input, start, len);
	input = del_substr(input, start, len);
	if (!res || !input)
		return (NULL);
	return (res);
}

// char *cut_tok(char *input, int start, int flag)
// {
// 	char *res;
// 	int	len;

// 	len = start;
// 	while (input[len] && !incl_char(input[len], " >|<"))
// 		len++;
// 	len -= start;
// 	res = ft_substr(input, start, len);
// 	input = del_substr(input, start, len);
// 	if (!res || !input)
// 		return (NULL);
// 	return (res);
// }