#include <cstdio>
#include <ctime>
#include <cmath>
#include <vector>
#include <list>
#include <algorithm>
#include "evolution.h"
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
	int targetNum = (input.getNumCusto() + input.calMinRoute() ) / 2;
	for(list<solution>::iterator it = population.begin(); it != population.end(); ++it){
		it->random(rand() % targetNum + 1, input);
	}
	
	for(int run = 0; run < 50; run++){
		list<solution> nextpopulation(0);

		for(int off = 0; off < 100; off++){
			solution p = crossover( tournament(population, input), tournament(population, input), input);
			nextpopulation.push_back(p);
		}

		population = nextpopulation;
	}

	for(list<solution>::const_iterator it = population.begin(); it != population.end(); ++it) if(it->feasible) it->print(stdout);



	return 0;
}