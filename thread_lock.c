#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
 
int sharei = 0;
void increase_num(void);
// add mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 
int main()
{
  int ret;
  pthread_t thread1,thread2,thread3;
  ret = pthread_create(&thread1,NULL,(void *)&increase_num,NULL);
  ret = pthread_create(&thread2,NULL,(void *)&increase_num,NULL);
  ret = pthread_create(&thread3,NULL,(void *)&increase_num,NULL);
 
  pthread_join(thread1,NULL);
  pthread_join(thread2,NULL);
  pthread_join(thread3,NULL);
 
  printf("sharei = %d\n",sharei);
 
  return 0;
}
 
void increase_num(void)
{
  long i,tmp;
  for(i =0;i<=10000;++i)
  {
    // lock
    if(pthread_mutex_lock(&mutex) != 0)
    {
      perror("pthread_mutex_lock");
      exit(EXIT_FAILURE);
    }
    tmp = sharei;
    tmp = tmp + 1;
    sharei = tmp;
    // unlock
    if(pthread_mutex_unlock(&mutex) != 0)
    {
      perror("pthread_mutex_unlock");
      exit(EXIT_FAILURE);
    }
  }
}
