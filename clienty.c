#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>



typedef struct sockaddr *sockaddrp;
int sockfd;

void *recv_other(void *arg)
{
    char buf[255]= {};
    while(1)
    {
        int ret = recv(sockfd,buf,sizeof(buf),0);
        if(0 > ret)
        {
            perror("recv");
            return;
        }
        printf("%s\n",buf);
    }
}




int main(int argc,char **argv)
{
    if(3 != argc)
    {
        perror("参数错误");
        return -1;
    }

    //建立socket对象
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(0 > sockfd)
    {
        perror("socket");
        return -1;
    }

    //准备连接地址
    struct sockaddr_in addr = {AF_INET};
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = inet_addr(argv[2]);

    socklen_t addr_len = sizeof(addr);


    //连接
    int ret = connect(sockfd,(sockaddrp)&addr,addr_len);
    if(0 > ret)
    {
        perror("connect");
        return -1;
    }

    //发送名字
    char buf[255] = {};
    char name[255] = {};
    printf("请输入您的昵称：");
    scanf("%s",name);
    ret = send(sockfd,name,strlen(name),0);
    if(0 > ret)
    {
        perror("connect");
        return -1;
    }

    //创建接收子线程
    pthread_t tid;
    ret = pthread_create(&tid,NULL,recv_other,NULL);
    
    if(0 > ret)
    {
        perror("pthread_create");
        return -1;
    }
    //循环发送
    while(1)
    {
        //printf("%s:",name);
        scanf("%s",buf);
        int ret = send(sockfd,buf,strlen(buf),0);
        if(0 > ret)
        {
            perror("send");
            return -1;
        }

        //输入quit退出
        if(0 == strcmp("quit",buf))
        {
            printf("%s,您已经退出了悟空聊天室\n",name);
            return 0;
        }

    }

}
