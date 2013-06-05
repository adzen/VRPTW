#include <stdio.h>
#include <stdlib.h>

/*     MAX MIN */
/* C1  104 101 */
/* C2  204 201 */
/* R1  108 101 */
/* R2  208 201 */
/* RC1 104 101 */
/* RC2 204 201 */

int main(){
   char filename[100];
   const char* type[] = {"C1", "C2", "R1", "R2", "RC1", "RC2"};
   int cusNum = 200;
   
   for(int typei = 0; typei < 6; ++typei){
   for(int testid = 1; testid <= 10; ++testid){
      sprintf(filename, "%04d_%s%02d.txt", cusNum, type[typei], testid);
      FILE *fp = fopen(filename, "r");
      
      int inta, intb;
      fscanf(fp, "%d%d", &inta, &intb);
      
      int sumOfTW = 0;
      for(int cus = 0; cus <= cusNum; cus++){
         for(int col = 0; col < 7; col++){
            fscanf(fp, "%d", &intb);
            if(col == 5) sumOfTW += intb - inta;   // length of time window
            inta = intb;
         }
      }
      fclose(fp);
      
      printf("%s : %8d\n", filename, sumOfTW);
   }}

   system("pause");
   return 0;
}
