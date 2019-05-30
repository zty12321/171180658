#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct sockaddr *sockaddrp;

//存储客户端地址的结构体数组
struct sockaddr_in src_addr[50];
socklen_t src_len = sizeof(src_addr[0]);



//连接后记录confd数组
int confd[50] = {};


//设置连接人数
int count = 0;


void *broadcast(void *indexp)
{
    int index = *(int *)indexp;
    char buf_rcv[255] = {};
    char buf_snd[255] = {};
    //第一次读取用户姓名
    char name[20] = {};
    int ret = recv(confd[index],name,sizeof(name),0);
    if(0 > ret)
    {
        perror("recv");
        close(confd[index]);
        return;
    }

    while(1)
    {
        bzero(buf_rcv,sizeof(buf_rcv));
        recv(confd[index],buf_rcv,sizeof(buf_rcv),0);

        //判断是否退出
        if(0 == strcmp("quit",buf_rcv))
        {
            sprintf(buf_snd,"%s已经退出聊天室",name);
            for(int i = 0;i < count;i++)
            {
                if(i == index || 0 == confd[i])
                {
                    continue;
                }

                send(confd[i],buf_snd,strlen(buf_snd),0);
            }
            confd[index] = -1;
            pthread_exit(0);
                    
        }


        sprintf(buf_snd,"%s:%s",name,buf_rcv);
        printf("%s\n",buf_snd);
        for(int i = 0;i <= count;i++)
        {
            if(i == index || 0 == confd[i])
            {
                continue;
            }

            send(confd[i],buf_snd,sizeof(buf_snd),0);
        }
        
    }

}





int main(int argc,char **argv)
{
    printf("聊天室服务器端开始运行\n");


    //创建通信对象
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(0 > sockfd)
    {
        perror("socket");
        return -1;
    }

    //准备地址
    struct sockaddr_in addr = {AF_INET};
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = inet_addr(argv[2]);

    socklen_t addr_len = sizeof(addr);



    //绑定
    int ret = bind(sockfd,(sockaddrp)&addr,addr_len);
    if(0 > ret)
    {
        perror("bind");
        return -1;
    }


    //设置最大排队数
    listen(sockfd,50);

    int index = 0;


    while(count <= 50)
    {
        confd[count] = accept(sockfd,(sockaddrp)&src_addr[count],&src_len);
        ++count;
        //保存此次客户端地址所在下标方便后续传入
        index = count-1; 
 
	pthread_t tid;
        int ret = pthread_create(&tid,NULL,broadcast,&index);
        if(0 > ret)
        {
            perror("pthread_create");
            return -1;
        }


    }


}
