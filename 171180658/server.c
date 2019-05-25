 /*此函数用于服务器端，暂时实现了退出功能，但判断昵称是否重复以及实现多人聊天并未实现
 可采取客服发往服务器，服务器转发消息至所有人的思路，即创建一个多维数组用以存储每个人发送到服务器的消息
 ，然后同时转发这些消息到所有人（除发送者）*/
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

#define PORT 15636

//void *thread(void *vargp);

void *threadsend(void *vargp);

void *threadrecv(void *vargp);

 
char temprecv[100];
char tempsend[100];
char namesend[100]={'\0'};
char namerecv[100]={'\0'};

int main()

{
int namelen=0;
struct sockaddr_in serveraddr; 
struct sockaddr_in clientaddr;
int clientlen, *connfd;

int listenfd = socket(AF_INET, SOCK_STREAM,0);

if(listenfd < 0){

        perror("socket");

        exit(1);

}

bzero((char *)&serveraddr,sizeof(serveraddr));

serveraddr.sin_family = AF_INET;

serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

serveraddr.sin_port = htons(PORT);



if(bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0){

        perror("connect");

        exit(1);

}



if(listen(listenfd,1024) < 0){

        perror("listen error");

        exit(1);

}





clientlen = sizeof(clientaddr);

connfd = (int *)malloc(sizeof(int));

*connfd = accept(listenfd,(struct sockaddr *)&clientaddr, &clientlen);

printf("Accepted!\n");

recv(*connfd,namerecv,100,0);
strcpy(namesend,namerecv);
send(*connfd,namesend,100,0);
for(int i=0;i<100;i++){
	if(namerecv[i]!='\0')
	namelen++;
}
namerecv[namelen-1]='\0';
printf("%s check in!\n",namerecv);

while(1){
	pthread_t tid1,tid2;
	
	pthread_create(&tid1,NULL,threadsend,connfd);
	
	if(tempsend[0]==':' && tempsend[1]== 'q'){
		printf("server close successfully!\n");
		close(*connfd);
		close(listenfd);
		return 0;
	}
	
	if(temprecv[0]==':' && temprecv[1]== 'q'){
		printf("client connection close\n");
		return 0;
	}
	
	pthread_create(&tid2,NULL,threadrecv,connfd);
}
return EXIT_SUCCESS;

}



//void *thread(void *vargp)
//
//{
//
//
//
//pthread_t tid1,tid2;
//
//
//pthread_create(&tid1,NULL,threadsend,vargp);
//
//pthread_create(&tid2,NULL,threadrecv,vargp);
//
//return NULL;
//
//}



void *threadsend(void * vargp)

{



int connfd = *((int *)vargp);


while(1){
	
fgets(tempsend,100,stdin);

send(connfd,tempsend,100,0);

if(tempsend[0]==':' && tempsend[1]=='q'){
	return NULL;
}
printf("----------------server\n");

memset(tempsend,'\0',100);

}

return NULL;

}



void *threadrecv(void *vargp)

{


int connfd = *((int *)vargp);

while(1){

int idata = 0;

idata = recv(connfd,temprecv,100,0);
if(temprecv[0]==':' &&temprecv[1]=='q'){
	return NULL;
}
if(idata > 0){

printf("%s :%s\n",namerecv,temprecv);

}

memset(temprecv,'\0',100);
}

return NULL;

}

