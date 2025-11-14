#include "app_buffer.h"
#include <stdio.h>
int main(void){
  BUFFER *buffer=  App_Buffer_Init(256);
//   App_Buffer_Write(buffer, "Hello",5);
//   App_Buffer_Write(buffer, "World",5);
//   char rec_buff1[6]= {0};
//   char rec_buff2[6]= {0};
//  int len1 = App_Buffer_Read(buffer, rec_buff1, 5);
//  int len2 = App_Buffer_Read(buffer, rec_buff2, 5);
//   printf("len1 = %d, rec_buff1 = %s\n", len1, rec_buff1);
//   printf("len2 = %d, rec_buff2 = %s\n", len2, rec_buff2);

//   char rec_buff3[6]= {0};
//   int len3 = App_Buffer_Read(buffer, rec_buff3, 5);
//   printf("len3 = %d, rec_buff3 = %s\n", len3, rec_buff3);
//    App_Buffer_Write(buffer, "Worldsdasadsdadasd",18);
  
  char str[255];
  for (int i = 0; i < 255; i++)
   { if (i==254)
   {
      
     str[i]='b';
   }
   else
     {str[i]='a';}
   }
  App_Buffer_Write(buffer, str,255);
   char rec_buff[255]={0};
   
  int len = App_Buffer_Read(buffer, rec_buff, 255);
  printf("len = %d, rec_buff = %s\n", len, rec_buff);
  return 0;
}