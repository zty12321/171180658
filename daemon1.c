#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main()
{
    pid_t pid;
    int i, fd;
    char *buf = "This is a Daemon!\n";

    if((pid=fork())<0)
    {
        printf("Fork error!\n");
        exit(1);
    }
    else if(pid>0)
        exit(0);
    
    setsid(); //Open new session
    chdir("/tmp");
    umask(0);
    
    for(i=0;i<getdtablesize();i++)
        close(i);

    while(1)
    {
        if((fd=open("daemon.log", O_CREAT|O_WRONLY|O_TRUNC, 0600))<0)
        {
            printf("Open file error!\n");
            exit(1);
        }
        write(fd, buf, strlen(buf));
        close(fd);
        sleep(2);
    }
    exit(0);
}
