#include <stdio.h>
#include <stdlib.h>

const char *type[] = {"C1", "C2", "R1", "R2", "RC1", "RC2"};
const char *size = "10";

int main(){
   char inputFile[100], token[100], outputFile[100];
   int customers = atoi(size) * 100;
   
   for(int typen = 0; typen < 6; ++typen){
   for(int no = 1; no <= 10; ++no){
      // inputfile
      sprintf(inputFile, "%s_%s_%d.txt", type[typen], size, no);
      freopen(inputFile, "r", stdin);
      
      // outputfile
      sprintf(outputFile, "%d_%s%02d.txt", customers, type[typen], no);
      freopen(outputFile, "w", stdout);
      
      // ignore something before capacity
      for(int i = 0; i < 6; ++i) scanf("%s", token);
      
      printf("%s\n", token);      // vehicle capacity
      printf("%d\n", customers);  // number of customers
      
      // ignore table header
      for(int i = 0; i < 12; ++i) scanf("%s", token);
      
      // just copy the content of table
      for(int customer = 0; customer <= customers; ++customer){
      for(int column = 0; column < 7; ++column){
         if(column > 0) printf("\t");
         scanf("%s", token);
         printf("%s", token);
         if(column == 6) printf("\n");
      }}
   }}

   return 0;
}
