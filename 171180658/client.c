/*client����ʵ����server�˷����ǳƲ�����server���ܣ������ɽ��в�����Ϣ���������� ��q �˳�����
����Ĭ���������ڳ�����ͨ�ţ���serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");����ֱ��ʹ����127.0.0.1��ip��ַ
��ʵ��ʹ�ù����У��Լ���������ʱ��Ҫ���˴���IP��ַ��Ϊ������ͬʱ���Ա�ʶ��ͬ�ͻ�*/ 
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
//����ȫ�ֱ��������÷ֱ�Ϊ������Ϣ��������Ϣ�������ǳơ������ǳ� 
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

//�˴������ж��ǳ��Ƿ��ظ�������server������Ӧλ�ý��ж�Զ��޸� 
while(flag){
printf("Please input your username:\n");
fgets(namesend,100,stdin);
send(*clientfdp,namesend,100,0);
recv(*clientfdp,namerecv,100,0);
flag=strcmp(namesend,namerecv);
}
printf("welcome,%s\n",namesend);

//��ѭ�� 
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


//������Ϣ�߳� 
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




//������Ϣ�߳� 
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

