#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    if(fork()==0)
    {
        if(execlp("/bin/ps", "ps", "-e", NULL) < 0)
            printf("execlp error!\n");
    }
    return 0;
}
