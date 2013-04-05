#include <cstdio>
#include <ctime>
#include <cmath>
#include <vector>
#include <list>
#include <algorithm>
#include "solution.h"
#include "problem.h"

using namespace std;

int main(int argc, char *argv[]){
	srand( (unsigned int)time(NULL) );

	// load input data
	problem input;
	if( !input.load("C:\\Users\\adzen\\Documents\\GitHub\\VRPTW\\problems\\200_R201.txt") ){
		puts("Unable to open input file!");
		return 1;
	}



	list<solution> population(100);
	int minRoute = input.calMinRoute();
	for(list<solution>::iterator it = population.begin(); it != population.end(); ++it){
		it->random(rand() % input.getNumCusto() + 1, input);
	}



	

	
	

	


	return 0;
}