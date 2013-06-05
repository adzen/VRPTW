#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <vector>

using namespace std;

enum {CUS_ID, X_POS, Y_POS, DEMAND, START, DEADLINE, UNLOAD};

void readSol(vector< vector<int> > &sol, const char *filename){
   FILE *fp = fopen(filename, "r");
   
   int numOfRoutes;
   fscanf(fp, "%d", &numOfRoutes);
   sol.resize(numOfRoutes);
   
   for(int routeid = 0; routeid < numOfRoutes; routeid++){
      int numOfCus;
      fscanf(fp, "%d", &numOfCus);
      
      for(int cusid = 0; cusid < numOfCus; cusid++){
         int id;
         fscanf(fp, "%d", &id);
         sol[routeid].push_back(id);
      }
   }
   
   fclose(fp);
}

void readInput(int *capacity, vector< vector<int> > &input, const char *filename){
   FILE *fp = fopen(filename, "r");

   fscanf(fp, "%d", capacity);
   
   int numOfCus;
   fscanf(fp, "%d", &numOfCus);
   input.resize(numOfCus + 1);  // with depot
   
   for(int cusid = 0; cusid <= numOfCus; cusid++){
      int in;
      for(int col = 0; col < 7; col++){
         fscanf(fp, "%d", &in);
         input[cusid].push_back(in);
      }
   }

   fclose(fp);
}

void checkOnce(vector< vector<int> > &sol, int numOfCus){
   vector<bool> isVisited(numOfCus + 1);
   
   for(unsigned int route = 0; route < sol.size(); route++){
      for(unsigned int cus = 0; cus < sol[route].size(); cus++){
         if( sol[route][cus] >= 1 && sol[route][cus] <= numOfCus ){
             if( isVisited[ sol[route][cus] ] == false ){
                isVisited[ sol[route][cus] ] = true;
             }else{
                printf("[X] customer %d is visited more than once.\n", sol[route][cus]);
             }
         }else{
            printf("[X] customer %d is invalid.\n", sol[route][cus]);
         }
      }
   }
   
   for(int cus = 1; cus <= numOfCus; cus++){
      if( isVisited[cus] == false ){
         printf("[X] customer %d is not visited.\n", cus);
      }
   }
}

void checkCapacity(vector< vector<int> > &sol, vector< vector<int> > &input, int capacity){
   for(unsigned int route = 0; route < sol.size(); route++){
      int load = 0;
      
      for(unsigned int cus = 0; cus < sol[route].size(); cus++){
         load += input[ sol[route][cus] ][DEMAND];     
      }
      
      if(load > capacity){
         printf("[X] route %d is overload. (%d / %d)\n", route, load, capacity);
      }
   }
}

double distance(vector< vector<int> > &input, int id1, int id2){
   return hypot(input[id1][1] - input[id2][1], input[id1][2] - input[id2][2]);
}

void checkTimeWindow(vector< vector<int> > &sol, vector< vector<int> > &input){
   double totalDistance = 0.0;
   
   for(unsigned int route = 0; route < sol.size(); route++){
      double arrival = distance(input, 0, sol[route][0]);
      totalDistance += distance(input, 0, sol[route][0]);
      
      for(unsigned int cus = 0; cus < sol[route].size(); cus++){
         double start = arrival;
         if(arrival > input[ sol[route][cus] ][DEADLINE]){
            printf("[X] route %d customer %d's arrival is late.\n", route, sol[route][cus]);
         }else if(arrival < input[ sol[route][cus] ][START]){
            start = input[ sol[route][cus] ][4];
         }
         
         double end = start + input[ sol[route][cus] ][UNLOAD];
         
         if(cus == sol[route].size() - 1){
            arrival = end + distance(input, sol[route][cus], 0);
            totalDistance += distance(input, sol[route][cus], 0);
            
            if(arrival > input[0][DEADLINE]){
               printf("[X] route %d can't back to depot in time.\n", route);
            }
         }else{
            arrival = end + distance(input, sol[route][cus], sol[route][cus+1]);
            totalDistance += distance(input, sol[route][cus], sol[route][cus+1]);
         }
      }
   }
   
   printf("%d routes, totalDistance = %.3f\n", sol.size(), totalDistance);
}

int main(){
   vector< vector<int> > sol;
   readSol(sol, "mysol.txt");
   
   int capacity;
   vector< vector<int> > input;
   readInput(&capacity, input, "0025_R105.txt");
   
   
   checkOnce(sol, input.size()-1);  // without depot
   checkCapacity(sol, input, capacity);
   checkTimeWindow(sol, input);
   
   
   system("pause");
   return 0;
}
