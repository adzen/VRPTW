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
