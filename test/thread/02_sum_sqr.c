#include "unistd.h"
#include "sys/time.h"
#include "stdio.h"
/** 
 * 串行执行
*/
int calc_sqr(int num) {
   int res= num*num;
   sleep(1);
   return res;
}
int main(){
   int nums[] = {1,2,3,4,5};
   int sqrs[5];
   struct timeval start, end;
   gettimeofday(&start,NULL);
   for ( int i = 0; i < 5; i++)
   {  
      sqrs[i] = calc_sqr(nums[i]);
   }
   gettimeofday(&end,NULL);
   printf("time: %ld\n", end.tv_sec - start.tv_sec);
   int sum=0;
   for ( int i = 0; i < 5; i++)
   {
      sum += sqrs[i];
   }
   printf("sum: %d\n", sum);
   return 0;
   
}