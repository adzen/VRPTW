#include <cstdio>
#include <cmath>
#include <cstdlib>

using namespace std;

const int size[] = {25, 50, 100, 200, 400, 600, 800, 1000};
const char *type[] = {"R1", "C1", "RC1", "R2", "C2", "RC2"};
const int solomon[] = {12, 9, 8, 11, 8, 8};
const int totalFiles = 5*6*10 + 3*56;

bool check(const char *filename, int size){
   printf("Checking input file %s... ", filename);
   
   FILE *fp = fopen(filename, "r");
   if(fp == NULL){
      printf("cannot open this file\n");
      return false;
   }
   
   int capacity;
   if( fscanf(fp, "%d", &capacity) != 1 ){
      printf("without capacity\n");
      fclose(fp);
      return false;
   }
   if( capacity <= 0 ){
      printf("capacity <= 0\n");
      fclose(fp);
      return false;
   }
   
   int customers;
   if( fscanf(fp, "%d", &customers) != 1 ){
      printf("without # of customers\n");
      fclose(fp);
      return false;
   }
   if( customers <= 0 ){
      printf("customers <= 0\n");
      fclose(fp);
      return false;
   }
   if( customers != size ){
      printf("# of customers didn't match with filename\n");
      fclose(fp);
      return false;
   }
   
   int cid, demand, dstart, dend, unload;
   double dx, dy;
   if( fscanf(fp, "%d%lf%lf%d%d%d%d", &cid, &dx, &dy, &demand, &dstart, &dend, &unload) != 7 ){
      printf("without depot\n");
      fclose(fp);
      return false;
   }
   if( cid != 0 ){
      printf("depot's id should be 0\n");
      fclose(fp);
      return false;
   }
   if( demand != 0 ){
      printf("depot's demand should be 0\n");
      fclose(fp);
      return false;
   }
   if( dstart > dend ){
      printf("depot's time window is invalid\n");
      fclose(fp);
      return false;
   }
   if( unload != 0 ){
      printf("depot's unload time should be 0\n");
      fclose(fp);
      return false;
   }
   
   for(int id = 1; id <= size; id++){
      int cstart, cend;
      double x, y;
      
      if( fscanf(fp, "%d%lf%lf%d%d%d%d", &cid, &x, &y, &demand, &cstart, &cend, &unload) != 7 ){
         printf("without customer #%d\n", id);
         fclose(fp);
         return false;
      }
      if( cid != id ){
         printf("customer #%d's id should be %d\n", cid, id);
         fclose(fp);
         return false;
      }
      if( demand < 0 || demand > capacity ){
         printf("customer #%d's demand is invalid\n", id);
         fclose(fp);
         return false;
      }
      if( cstart > cend || cstart < dstart || cend > dend ){
         printf("customer #%d's time window is invalid\n", id);
         fclose(fp);
         return false;
      }
      if( unload < 0 || unload > dend-dstart ){
         printf("customer #%d's unload time is invalid\n", id);
         fclose(fp);
         return false;
      }
      
      double arrive = hypot(x-dx, y-dy);
      if(arrive > cend){
         printf("customer #%d cannot be arrived in time\n", id);
         fclose(fp);
         return false;
      }
      if(arrive < cstart) arrive = cstart;
      arrive += unload;
      arrive += hypot(x-dx, y-dy);
      if(arrive > dend){
         printf("customer #%d cannot back to depot in time\n", id);
         fclose(fp);
         return false;
      }
   }
   
   printf("OK\n");
   fclose(fp);
   return true;
}

int main(){
   int errors = 0, checkedFile = 0;
    
   for(int typen = 0; typen < 6; typen++){
   for(int sizen = 0; sizen < 8; sizen++){
      char filename[100];
      
      if(sizen < 3){
         // solomon's
         for(int no = 1; no <= solomon[typen]; no++){
            sprintf(filename, "%04d_%s%02d.txt", size[sizen], type[typen], no);
            if( !check(filename, size[sizen]) ) errors++;
            checkedFile++;
         } 
      }else{
         // G&H's
         for(int no = 1; no <= 10; no++){
            sprintf(filename, "%04d_%s%02d.txt", size[sizen], type[typen], no);
            if( !check(filename, size[sizen]) ) errors++;
            checkedFile++;
         }
      }
   }}
   
   if(checkedFile != totalFiles){
      printf("Error: # of checked file = %d / %d\n", checkedFile, totalFiles);
      errors++;
   }else{
      printf("%d files checked\n", totalFiles);
   }

   printf("%d errors\n", errors);
   system("pause");
   return 0;
}
