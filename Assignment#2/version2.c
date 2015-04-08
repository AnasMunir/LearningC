#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


char *cmd2[] = {"grep","e", NULL};
char *cmd1[] = {"tail","-5", "/etc/passwd", NULL};

int main(void)
{
	pid_t pid_1, pid_2;
	int fd_1[2], fd_2[2];

	pipe(fd_1);/*construct of our first pipe*/

	int newfd = open("YoYo.txt", CREATE_FLAGS, CREATE_MODE);
        dup2(newfd, STDOUT_FILENO);
        close(newfd);

	switch(pid_1 = fork()) {	/* the birth of our first child */

		case 0:		/* In our first child same as if(pid_1 == o) */
			
			pipe(fd_2);	/* constructing our secnd pipe */
			pid_2 = fork();	/* Our second/grand child */
            if(pid_2 == 0){

            	close(fd_1[1]), close(fd_1[0]);

				dup2(fd_2[1], 1);
				close(fd_2[1]), close(fd_2[0]);
				execvp(cmd1[0], cmd1);
				perror("Failed bro");
				wait(NULL) != pid_2;

			} else { /*now again, in our first child*/
				
				dup2(fd_2[0], 0);
				close(fd_2[0]), close(fd_2[1]);

				dup2(fd_1[1], 1);
				close(fd_1[1]), close(fd_1[0]);
				execvp(cmd2[0], cmd2);
				perror(cmd2[0]);	

				wait(NULL) != pid_1;
			}
			break;		

		default:/*now in our parent process*/
			
			dup2(fd_1[0], 0);
			close(fd_1[0]), close(fd_1[1]);
			execl("/usr/bin/sort", "sort", "-n", NULL);
			//execvp(cmd3[0], cmd3);

			break;

		case -1:
			perror("fork");
			exit(1);

	}
	
	exit(0);
}


