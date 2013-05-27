#include <vector>
#include <algorithm>
#include "evolution.h"

using namespace std;

solution crossover(const solution &pa, const solution &pb, const problem& input){
	solution offspring;
	
	vector<route> allRoutes;
	allRoutes.insert(allRoutes.end(), pa.routes.begin(), pa.routes.end() );
	allRoutes.insert(allRoutes.end(), pb.routes.begin(), pb.routes.end() );

	int remainCus = input.getNumCusto();
	vector<bool> insertedCus(input.getNumCusto()+1);

	while(remainCus > 0 && allRoutes.size() > 0){
		int selectRouteA = rand() % allRoutes.size();
		int selectRouteB = rand() % allRoutes.size();
		int cmpResult = route::cmp(allRoutes[selectRouteA], allRoutes[selectRouteB], input);
		
		if(cmpResult == 0) cmpResult = (rand() % 2) * 2 - 1;

		route deleteRoute;
		if(cmpResult < 0){
			offspring.routes.push_back(allRoutes[selectRouteA]);
			deleteRoute = allRoutes[selectRouteA];
			allRoutes.erase(allRoutes.begin() + selectRouteA);
		}else{
			offspring.routes.push_back(allRoutes[selectRouteB]);
			deleteRoute = allRoutes[selectRouteB];
			allRoutes.erase(allRoutes.begin() + selectRouteB);
		}

		remainCus -= deleteRoute.visits.size();
		for(list<int>::const_iterator it = deleteRoute.visits.begin(); it != deleteRoute.visits.end(); it++){
			insertedCus[ (*it) ] = true;
			
			for(unsigned int i = 0; i < allRoutes.size(); ++i){
				if( allRoutes[i].hasCus( (*it) ) ){
					allRoutes.erase(allRoutes.begin() + i);
					--i;
				}
			}	
		}
	}

	if(remainCus > 0){
		route additional;
		vector<int> newVisits;
		for(int i = 1; i <= input.getNumCusto(); ++i){
			if(insertedCus[i] == false){
				newVisits.push_back(i);
			}
		}
		random_shuffle(newVisits.begin(), newVisits.end());
		additional.visits = list<int>(newVisits.begin(), newVisits.end());
		offspring.routes.push_back(additional);
	}

	offspring.fitness(input);
	return offspring;
}

const solution& tournament(const std::list<solution> &population, const problem &input){
	int selectA = rand() % population.size();
	int selectB = rand() % population.size();

	list<solution>::const_iterator itA = population.begin();
	advance(itA, selectA);
	list<solution>::const_iterator itB = population.begin();
	advance(itB, selectB);

	int cmp = solution::cmp( (*itA), (*itB), input);
	if(cmp == 0) cmp = (rand() % 2) * 2 - 1;

	if(cmp < 0) return (*itA);
	else return (*itB);
}

// Use Deb's "Fast Nondominated Sorting" (2002)
// Ref.: "A fast and elitist multiobjective genetic algorithm: NSGA-II"
void ranking(const std::list<solution> &population, std::vector< std::list<solution> > *output){
	vector<solution> solutions(population.begin(), population.end() );

	vector< list<int> > intOutput;
	intOutput.resize(solutions.size() + 2);  // start from 1, end with null Qi 
	output->resize(1);

	// record each solutions' dominated solution
	vector< list<int> > dominated;
	dominated.resize(solutions.size());

	// record # of solutions dominate this solution
	vector<int> counter;
	counter.resize(solutions.size());

	// record the rank of solutions
	vector<int> rank;
	rank.resize(solutions.size());

	
	// for each solution
	for(unsigned int p = 0; p < solutions.size(); p++){
		for(unsigned int q = 0; q < solutions.size(); q++){
			if( solution::dominate(solutions[p], solutions[q]) ){
				dominated[p].push_back(q);  // Add q to the set of solutions dominated by p
			}else if( solution::dominate(solutions[q], solutions[p]) ){
				counter[p]++;
			}
		}
		
		if(counter[p] == 0){  // p belongs to the first front
			rank[p] = 1;
			intOutput[1].push_back(p);
			(*output)[0].push_back(solutions[p]);
		}
	}

	int curRank = 1;
	while( intOutput[curRank].size() > 0 ){
		list<int> Qi;  // Used to store the members of the next front
		list<solution> Qs;

		for(list<int>::iterator p = intOutput[curRank].begin(); p != intOutput[curRank].end(); p++){
		for(list<int>::iterator q = dominated[(*p)].begin(); q != dominated[(*p)].end(); q++){
			counter[(*q)]--;
			if(counter[(*q)] == 0){  // q belongs to the next front
				rank[(*q)] = curRank + 1;
				Qi.push_back(*q);
				Qs.push_back(solutions[(*q)]);
			}
		}}

		curRank++;
		intOutput[curRank] = Qi;
		if(Qi.size() > 0) output->push_back(Qs);
	}
}
