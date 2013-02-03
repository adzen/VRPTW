#include <stdio.h>
#include <stdlib.h>

const int instance_num = 8;

int main(){
   freopen("input.txt", "r", stdin);
   
   int test, i, j;
   char token[100], filename[100];
   for(test = 0; test < instance_num; ++test){
      scanf("%s", token);  // name
      sprintf(filename, "%s.txt", token);
      freopen(filename, "w", stdout);
      
      for(i = 0; i < 11; ++i) scanf("%s", token);   // ignore header
      
      puts("1000");   // capacity
      puts("100");
      
      for(i = 0; i <= 100; ++i){
         printf("%d", i);   // customer's id
         
         for(j = 0; j <= 6; ++j){
            scanf("%s", token);
            if(j) printf("\t%d", atoi(token) );
         }
         
         puts("");
      }   
   }

   return 0;
}
