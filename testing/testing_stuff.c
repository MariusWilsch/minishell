/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing_stuff.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 11:32:39 by verdant           #+#    #+#             */
/*   Updated: 2023/04/04 11:53:15 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>


extern char **environ;

// int	main(void)
// {
// 	char *argv[] = {"test=1", "test2=2", "test3=3", NULL};
// 	int	argc = 3;
// 	int i;

// 	if (argc == 1)
// 	{
// 		printf("No arguments passed\n");
// 		return (0);
// 	}
// 	for (i = 0; environ[i]; i++)
// 		printf("%d\t%s\n", i, environ[i]);
// 	environ[i] = "test=1";
// 	environ[i + 1] = NULL;
// 	for (i = i; environ[i]; i++)
// 		printf("%d\t%s\n", i, environ[i]);
// }

int main() {
    int i = 0;

    while (i < 10) {
        if (i == 5) {
            printf("skipping i = %d\n", i);
            continue;
        }
        printf("i = %d\n", i);
        i++;
    }

    return 0;
}