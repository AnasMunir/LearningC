#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/* tail -5 /etc/passwd | grep e | sort */
char *cmd2[] = {"grep","e", NULL};
char *cmd1[] = {"tail","-5", "/etc/passwd", NULL};

int main(void)
{
    int pid_1, pid_2;
    int fd1[2], fd2[2]; 
    
    int newfd;

    newfd = open("result.txt", CREATE_FLAGS, CREATE_MODE);
        dup2(newfd, STDOUT_FILENO);
        close(newfd);

    if((pipe(fd1) == -1) || ((pid_1 = fork()) == -1)) {
        perror("Failed to setup 1st pipeline");
        return 1;
    }

    if (pid_1 == 0) { 

        if((pipe(fd2) == -1) || ((pid_2 = fork()) == -1)){ 
            perror("Failed to setup 2nd pipeline");
            return 1;
        }
        if (pid_2 == 0) {

            if ((close (fd1[1]) == -1) ||  (close (fd1[0]) == -1)){
            perror("fail to close the first pipe");
            return 1;
            }

            if(dup2(fd2[1],1) == -1)
                perror("Failed in pid_2");
            else if ((close(fd2[1]) == -1) || (close(fd2[0]) == -1))
                perror("failed");
            else {
                execvp(cmd1[0], cmd1);
                perror("execvp second child");
            }
            return 1;
        } //end of second child

            if(dup2(fd2[0],0) == -1)
                perror("Failed for std in");
            if ((close(fd2[0]) == -1) || (close(fd2[1]) == -1))
                perror("failed");
           

            if(dup2(fd1[1], 1) == -1)
                perror("failed for std_out");
            else if ((close(fd1[1]) == -1) ||  (close(fd1[0]) == -1))
                perror("failed");
            else {
                execvp(cmd2[0], cmd2);
                perror(cmd2[0]);
            }
            return 1;
    } //end of first child
    
        if(dup2(fd1[0],0) == -1)
            perror("failed");
        else if ((close (fd1[0]) == -1) ||  (close (fd1[1]) == -1))
            perror(" failed");
        else {
           
            execl("/usr/bin/sort", "sort", NULL);
            perror("execl father"); 
        }
        return 1; 
    } 
