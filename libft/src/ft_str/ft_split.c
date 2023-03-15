/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 17:54:34 by mwilsch           #+#    #+#             */
/*   Updated: 2023/03/15 14:38:13 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>


// static size_t	count_words(char *s, char c)
// {
// 	int	cnt;
// 	int	i;

// 	cnt = 0;
// 	i = 0;
// 	while (s[i])
// 	{
// 		if (s[i] != c)
// 		{
// 			cnt++;
// 			while (s[i] != c && s[i + 1])
// 				i++;
// 		}
// 		i++;
// 	}
// 	return (cnt);
// }

// static void	free_mem(char **arr, size_t i)
// {
// 	size_t	j;

// 	j = 0;
// 	while (j < i)
// 		free(arr[j++]);
// 	free(arr);
// }

// static char	*store_wrd(const char *s, char c, char **arr, size_t i)
// {
// 	size_t	len_word;
// 	char	*s_temp;

// 	len_word = 0;
// 	while (*s && *s != c)
// 	{
// 		len_word++;
// 		s++;
// 	}
// 	s_temp = ft_substr(s - len_word, 0, len_word);
// 	if (!s_temp)
// 	{
// 		free_mem(arr, i);
// 		*arr = NULL;
// 	}
// 	return (s_temp);
// }

// char	**ft_split(char const *s, char c)
// {
// 	char	**split_pointers;
// 	size_t	i;
// 	size_t	word_count;
// 	char	*s_temp;

// 	i = 0;
// 	word_count = count_words((char *)s, c);
// 	s_temp = (char *)s;
// 	split_pointers = ft_calloc((word_count + 1), sizeof(char *));
// 	if (!s || !split_pointers)
// 		return (NULL);
// 	while (i < word_count)
// 	{
// 		if (*s != c)
// 		{
// 			s_temp = store_wrd(s, c, split_pointers, i);
// 			split_pointers[i++] = s_temp;
// 			s = s + ft_strlen(s_temp);
// 		}
// 		else
// 			s++;
// 	}
// 	return (split_pointers);
// }

int	count_words(char *str, char c)
{
	int i;
	int	cnt;

	i = 0;
	cnt = 0;
	if (str == NULL)
		return (-1);
	while (str[i])
	{
		if (str[i] != c) // Reached a word
		{
			cnt++;
			while (str[i] != c && str[i + 1] != '\0') // Skip over the chars in a word
				i++;
		}
		i++; // Iterating over delimiteres
	}
	return (cnt);
}

int	count_chars(char *str, char c)
{
	int	cnt;
	int	i;

	cnt = 0;
	i = 0;
	while (str[i] && str[i] == c) // Skip over delimiter
		i++;
	while (str[i] && str[i] != c) // Count Chars
	{
		i++;
		cnt++;
	}
	return (cnt);
}

char *store_wrd(char *str, char c)
{
	const int	word_len = count_chars(str, c); // Count the chars
	char *temp;
	int	i;
	int	k;
	
	i = 0;
	k = 0;
	temp = malloc(sizeof(char) * (word_len + 1));
	if (!temp)
		return (NULL);
	temp[word_len] = '\0';
	while (str[i] && str[i] == c) // Skip over delimiter
		i++;
	while (str[i] && k < word_len)
		temp[k++] = str[i++];
	return (temp);
}

void	trim_string(char *str, char c)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (str[i] && str[i] == c) // Skip over delimiter
		i++;
	i += count_chars(str, c);
	while (str[i]) // Copy over the remaning stuff
		str[k++] = str[i++];
	str[k] = '\0';
}

char **ft_split(char const *s, char c)
{
	int		wrd_cnt = count_words((char *)s, c);
	char	**s_arr;
	char	*str = ft_strdup(s);
	int		i = 0;

	s_arr = malloc(sizeof(char *) * (wrd_cnt + 1));
	if (!s_arr || wrd_cnt == -1)
		return (free(s_arr), NULL); // Do I need to free here?
	s_arr[wrd_cnt] = NULL;
	while (i < wrd_cnt)
	{
		s_arr[i] = store_wrd(str, c);
		trim_string(str, c);
		i++;
	}
	return (free(str), s_arr);
}

// void leaks(void)
// {
// 	system("leaks -q a.out");
// }

// int main(int argc, char *argv[])
// {
// 	char **arr = ft_split(argv[1], ' ');
// 	int i = 0;
// 	while (arr[i])
// 	{
// 		printf("%s\n", arr[i]);
// 		i++;
// 	}
// 	printf("%s", arr[i]);
// }
