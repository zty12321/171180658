 #include<stdio.h>
   #include<stdlib.h>
   #include<pthread.h>

   int global = 0;//定义全局变量
   //加入互斥锁解决线程冲突
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;//定义全局锁并初始化
  void* pthread_add(void* val)
  {
      int i = 2000;
      while(i--)
      {
          pthread_mutex_lock(&lock);//加锁
          int tmp = global;
          printf("%d\n",global);
          global = tmp + 1;
          pthread_mutex_unlock(&lock);//解锁
      }
      return (void*)0;
  }
  int main()
  {
      pthread_t tid1;
      pthread_t tid2;
      pthread_create(&tid1,NULL,pthread_add,NULL);//创建线程1
      pthread_create(&tid2,NULL,pthread_add,NULL);//创建线程2
      pthread_join(tid1,NULL);//等待线程1
      pthread_join(tid2,NULL);//等待线程2
      printf("the global is %d\n",global);
      pthread_mutex_destroy(&lock);//销毁锁资源
      return 0;
  }
