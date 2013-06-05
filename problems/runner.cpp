#include <stdio.h>
#include <stdlib.h>

const char *type[] = {"R1", "C1", "RC1", "R2", "C2", "RC2"};
const int test[] = {12,9,8,11,8,8};

int main(){
   for(int typec = 0; typec < 6; typec++){
   for(int testc = 1; testc <= test[typec]; testc++){
      char filename[100];
      sprintf(filename, "0025_%s%02d.txt", type[typec], testc);
      rename(filename, "input.txt");
      puts(filename);
      system("VRPTW.exe");
      rename("input.txt", filename);
      system("pause");
   }}
   
   return 0;
}
