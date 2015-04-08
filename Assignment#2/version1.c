#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
main()
{
    pid_t pid1, pid2;
    int fd1[2];
    int fd2[2];
    char *arg[4];
    char *arg2[3];
    char *arg3[3];  
    
   
    /* tail -5 /etc/passwd | grep e | sort */
    /* -----process 1------|process2|prcoess3*/
    int newfd = open("YoYo.txt", CREATE_FLAGS, CREATE_MODE);
        dup2(newfd, STDOUT_FILENO);
        close(newfd);

    pipe(fd1); 
    pid1 = fork();

    if (pid1 == 0) { /* Child process 2 */
        
        pipe(fd2);
      
        pid2=fork();
    
        if (pid2 == 0) { /* grandchild process 1*/
            
            close (fd1[1]), close (fd1[0]);
            
            
            dup2 (fd2[1],1);
            close (fd2[1]), close (fd2[0]);
            arg[0] = "tail";
            arg[1] = "-5";
            arg[2] = "/etc/passwd";
            arg[3] = NULL;
            execvp("tail", arg);
            perror("execl second child");

            wait(NULL) != pid2;
        }else { /* executing child process 2 */
        
            dup2(fd2[0],0);
            close (fd2[0]), close (fd2[1]);
            
            dup2(fd1[1],1);
            close (fd1[1]), close (fd1[0]);
            arg2[0] = "grep";
            arg2[1] = "e";
            arg2[2] = NULL;
            execvp("grep", arg2);
            perror("execl first child");
            wait(NULL) != pid1;
        }
    }
    else { /* executing parent process 3 */
        
        dup2(fd1[0],0);
        close (fd1[0]), close (fd1[1]);
        execl("/usr/bin/sort", "sort", "-n", NULL);
        perror("execl father");    
    }
} 
