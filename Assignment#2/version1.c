#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
main()
{
    int pid, pid2;
    int fd[2];
    int fd2[2];
    char *arg[4];
    char *arg2[3];
    char *arg3[3];  
    
    /* cat /etc/passwd| grep /bin/bash$ | wc-l */
    /* tail -5 /etc/passwd | grep e | sort */

    int newfd = open("YoYo.txt", CREATE_FLAGS, CREATE_MODE);
        dup2(newfd, STDOUT_FILENO);
        close(newfd);

    pipe(fd); 
    pid = fork();

    if (pid == 0) {//first child
        
        pipe(fd2);
      
        pid2=fork();
    
        if (pid2 == 0) {
            //i am child 2 (child of the child)
            close (fd[1]), close (fd[0]);
            
            
            dup2 (fd2[1],1);
            close (fd2[1]), close (fd2[0]);
            arg[0] = "tail";
            arg[1] = "-5";
            arg[2] = "/etc/passwd";
            arg[3] = NULL;
            execvp("tail", arg);
            perror("execl second child");

            wait(NULL) != pid2;
        }else {
            //i am child 1
            

            dup2(fd2[0],0);
            close (fd2[0]), close (fd2[1]);
            
            
            
            dup2(fd[1],1);
            close (fd[1]), close (fd[0]);
            arg2[0] = "grep";
            arg2[1] = "e";
            arg2[2] = NULL;
            execvp("grep", arg2);
            perror("execl first child");
            wait(NULL) != pid;
        }
    }
    else {
        //i 'm the father
        
        dup2(fd[0],0);
        close (fd[0]), close (fd[1]);

        execl("/usr/bin/sort", "sort", "-n", NULL);

        
        //execl("cat", "/etc/passwd", NULL);
        perror("execl father");    
    }
} 