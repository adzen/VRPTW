#include <cstdio>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>
#include "evolution.h"
#include "solution.h"
#include "problem.h"

using namespace std;

int main(int argc, char *argv[]){
	clock_t start = clock();
	srand( (unsigned int)time(NULL) );

	// load input data
	problem input;
	if( !input.load("input.txt"/*"C:\\Users\\adzen\\Documents\\GitHub\\VRPTW\\problems\\0025_RC208.txt"*/) ){
		puts("Unable to open input file!");
		return 1;
	}
	
	// initial population
	list<solution> population(100);
	int targetNum = (input.getNumCusto() + input.calMinRoute() ) / 2, initID = 0;
	for(list<solution>::iterator it = population.begin(); it != population.end(); ++it, ++initID){
		if(initID < 50) it->random(input);
		else it->random(targetNum, input);
	}
	
	// evolution
	for(int run = 0; run < 100; run++){
		list<solution> merged(population.begin(), population.end() );
		for(int off = 0; off < 100; off++){
			solution p = crossover( tournament(population, input), tournament(population, input), input);
			merged.push_back(p);
		}

		for(list<solution>::iterator it = merged.begin(); it != merged.end(); it++) mutation(*it, input);

		vector< list<solution> > rank;
		ranking(merged, &rank);

		population.clear();
		environmental(rank, &population, 100);
	}

	// finally...
	vector< list<solution> > rank;
	ranking(population, &rank);
	rank[0].sort(solution::sort);
	rank[0].unique(solution::isSame);

	for(list<solution>::const_iterator it = rank[0].begin(); it != rank[0].end(); ++it) if(it->feasible) it->print(stdout);
	clock_t end = clock();

	printf("%f second\n", ((double)(end)-start) / CLOCKS_PER_SEC);
	system("pause");
	return 0;
}