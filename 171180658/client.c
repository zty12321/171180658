/*client函数实现向server端发送昵称并登入server功能，登入后可进行并发消息操作，输入 ：q 退出聊天
这里默认是主机内程序互相通信，故serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");这里直接使用了127.0.0.1的ip地址
在实际使用过程中，以及多人聊天时，要将此处的IP地址改为变量，同时用以辨识不同客户*/ 
#include <stdlib.h>

#include <stdio.h>

#include <errno.h>

#include <string.h>

#include <unistd.h>

#include <netdb.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <sys/types.h>

#include <arpa/inet.h>

#include <pthread.h>

 

#define MAXLINE 100;

void *threadsend(void *vargp);

void *threadrecv(void *vargp);
//定义全局变量，作用分别为接收信息、发送信息、发送昵称、接收昵称 
char temprecv[100];
char tempsend[100];
char namesend[100]={'\0'};
char namerecv[100]={'\0'};

int main()

{
int *clientfdp;
struct sockaddr_in serveraddr;
pthread_t tid1,tid2;
int flag=1;

clientfdp = (int *)malloc(sizeof(int));

*clientfdp = socket(AF_INET,SOCK_STREAM,0);

bzero((char *)&serveraddr,sizeof(serveraddr));

serveraddr.sin_family = AF_INET;

serveraddr.sin_port = htons(15636);

serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

if(connect(*clientfdp,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0){

        printf("connect error\n");

        exit(1);

}

printf("connected\n");

//此处用以判断昵称是否重复，须在server函数对应位置进行多对多修改 
while(flag){
printf("Please input your username:\n");
fgets(namesend,100,stdin);
send(*clientfdp,namesend,100,0);
recv(*clientfdp,namerecv,100,0);
flag=strcmp(namesend,namerecv);
}
printf("welcome,%s\n",namesend);

//主循环 
while(1){

pthread_create(&tid1,NULL,threadsend,clientfdp);

if(tempsend[0]==':' && tempsend[1]=='q'){
	printf("client quit successfully!\n");
	close(*clientfdp);
	return 0;
}
if(temprecv[0]==':' && temprecv[1]=='q'){
	printf("server loss connection\n");
	return 0;
}
pthread_create(&tid2,NULL,threadrecv,clientfdp);

}



return EXIT_SUCCESS;

}


//发送信息线程 
void *threadsend(void * vargp)

{

//pthread_t tid2;

int connfd = *((int *)vargp);



int idata;


while(1){

//printf("me: \n ");

fgets(tempsend,100,stdin);

send(connfd,tempsend,100,0);
if(tempsend[0]==':' && tempsend[1]=='q'){
	return 0;
}

memset(tempsend,'\0',100);
printf("----------------%s\n",namesend);



}

return NULL;

}




//接收信息线程 
void *threadrecv(void *vargp)

{


int connfd = *((int *)vargp);

while(1){

int idata = 0;

idata = recv(connfd,temprecv,100,0);
if(temprecv[0]==':' && temprecv[1]=='q'){
	return 0;
}
if(idata > 0){

printf("server :%s\n",temprecv);

}
memset(temprecv,'\0',100);
}

 

 

return NULL;

}

