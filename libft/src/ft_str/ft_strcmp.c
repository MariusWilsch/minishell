/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 12:11:27 by verdant           #+#    #+#             */
/*   Updated: 2023/04/19 12:12:02 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_strcmp(const char *str1, const char *str2)
{
		size_t index;

		index = 0;
		// Continue comparing characters until a null-terminator is encountered in either str1 or str2
		while (str1[index] || str2[index])
		{
				// If the current character in str1 is greater than the one in str2, return 1
				if ((unsigned char)str1[index] > (unsigned char)str2[index])
						return 1;

				// If the current character in str2 is greater than the one in str1, return -1
				if ((unsigned char)str2[index] > (unsigned char)str1[index])
						return -1;

				// Increment the index to compare the next pair of characters
				index++;
		}

		// If the loop exits, it means both strings are equal, so return 0
		return 0;
}
