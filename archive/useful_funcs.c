/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useful_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwilsch <mwilsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:06:53 by verdant           #+#    #+#             */
/*   Updated: 2023/03/24 10:07:54 by mwilsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <unistd.h>
#include <fcntl.h>

// /**
//  * @brief
//  *
//  * @note A pipe is created with the pipe() system call, which provides a pair of file descriptors
//  * @note 1. pipefd[0] for reading
//  * @note 2. pipefd[1] for writing
//  */

// int main(void)
// {
//   // Create a pipe between two commands (e.g., cmd1 | cmd2)
//   int pipefd[2];
//   pipe(pipefd);
//   char *cmd1_args[] = {"ls", "-l", NULL};
//   char *cmd2_args[] = {"wc", NULL};
// 	// fork and execve to run cmd1 and cmd2
//   pid_t pid = fork();
//   if (pid == 0)   // Child process (running cmd1)
//   {
//       close(pipefd[0]); // The read end of the pipe (pipefd[0]) is closed, as it’s not needed.
//       dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to the write end of the pipe
//       close(pipefd[1]);
//       execve("/bin/ls", cmd1_args, NULL);
//   }
//   else      // Parent process (running cmd2)
//   {
//       close(pipefd[1]); // Close the write end of the pipe
//       dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to the read end of the pipe
//       close(pipefd[0]); // The read end of the pipe (pipefd[0]) is closed, as it’s not needed after the dup2 call.
//       execve("/usr/bin/wc", cmd2_args, NULL);
//   }
// }

// int main1(void)
// {
// 	// Logic of cmd > file
// 	int fd1 = open("str", O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 	// Logic of cmd >> file
//   int fd2 = open("str", O_CREAT | O_WRONLY | O_APPEND, 0644);
// 	// Logic of cmd < input
// 		// ??
// 	// Logic of cmd << EOF
// 		// ??
// }

#include <stdio.h>
#include <unistd.h>

int main() {
    char *args[] = {"/bin/ls", "-l", NULL};
    char *envp[] = {NULL};

    int ret = execve("/bin/ls", args, envp);
    if (ret == -1) {
        perror("execve");
        return 1;
    }

    return 0;
}