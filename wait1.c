#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t pid, ret;
    if((pid=fork())<0)
        printf("Fork error!\n");
    else if(pid==0)
    {
        sleep(5);
        exit(0);
    }
    else{
        do
        {
            ret = waitpid(pid, NULL, WNOHANG);
            if(ret == 0)
            {
                printf("The child process has not exited!\n");
                sleep(1);
            }
        }
        while(ret==0);
        if(pid==ret)
            printf("Child process exited!\n");
        else
            printf("Some error occured!\n");
    }
}

