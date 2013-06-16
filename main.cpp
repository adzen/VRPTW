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
	// commands: VRPTW.exe inputFileName randomSeed(optional)
	if(argc < 2){
		puts("Unknown commands.");
		return 1;
	}

	clock_t start = clock();
	unsigned int seed;
	if(argc > 2){
		seed = (unsigned int)atoi(argv[2]);
	}else{
		seed = (unsigned int)time(NULL);
	}
	printf("Seed = %d\n", seed);
	srand(seed);

	// load input data
	problem input;
	if( !input.load(argv[1]) ){
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
	for(int run = 0; run < 500; run++){
		putchar('*');

		list<solution> merged(population.begin(), population.end() );
		for(int off = 0; off < 50; off++){
			solution parent1 = tournament(population, input);
			solution parent2 = tournament(population, input);

			solution off1 = crossover(parent1, parent2, input);
			merged.push_back(off1);
			solution off2 = crossover(parent2, parent1, input);
			merged.push_back(off2);
		}

		list<solution> feasible, infeasible;
		for(list<solution>::iterator it = merged.begin(); it != merged.end(); it++){
			mutation(*it, input);

			if(it->feasible) feasible.push_front(*it);
			else infeasible.push_front(*it);
		}

		vector< list<solution> > frank, irank;
		ranking(feasible, &frank, true);
		ranking(infeasible, &irank, false);

		population.clear();
		environmental(frank, irank, &population, 100);
	}
	putchar('\n');

	// finally...
	vector< list<solution> > rank;
	ranking(population, &rank, true);
	rank[0].sort(solution::sort);
	rank[0].unique(solution::isSame);

	for(list<solution>::const_iterator it = rank[0].begin(); it != rank[0].end(); ++it)
		if(it->feasible) it->print(stdout);
	clock_t end = clock();

	printf("%.3f second\n", ((double)(end)-start) / CLOCKS_PER_SEC);
	return 0;
}