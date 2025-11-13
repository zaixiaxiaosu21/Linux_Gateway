#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>

int calc_sqr(int num) {
   int res= num*num;
   sleep(1);
   return res;
}
void *thread_fun(void *arg) {
    int num = *(int *)arg;
    int *result = malloc(sizeof(int));
    *result = calc_sqr(num);
    return result;
}
int main(){
   int nums[] = {1,2,3,4,5};
   int sqrs[5];
   struct timeval start, end;
   gettimeofday(&start,NULL);
   pthread_t threads[5];
   for ( int i = 0; i < 5; i++)
   {  
      pthread_create(&threads[i],NULL, thread_fun,&nums[i]);
      
   }
   gettimeofday(&end,NULL);
   printf("time: %ld\n", end.tv_sec - start.tv_sec);
   int sum=0;
   //等待所有线程结束
   for ( int i = 0; i < 5; i++){
      void *res;
      pthread_join(threads[i], &res);
      sqrs[i] = *(int *)res;
      sum+=sqrs[i];
   }
   printf("sum: %d\n", sum);
   return 0;
   
}