#include <algorithm>

#include "solution.h"

using namespace std;

void solution::print(FILE *fp) const {
	fprintf(fp, "[%d routes, distance = %.3f]\n", routes.size(), totalDistance);
	for(list<route>::const_iterator it = routes.begin(); it != routes.end(); ++it){
		it->print(fp);
	}
}

void solution::clear(){
	routes.clear();
	totalDistance = totalTimewarp = 0;
}

void solution::random(int maxRoutes, const problem& input){
	clear();

	vector< vector<int> > newRoutes;
	newRoutes.resize(maxRoutes);

	for(int id = 1; id <= input.getNumCusto(); ++id){
		newRoutes[ rand() % maxRoutes ].push_back(id);
	}

	for(int i = 0; i < maxRoutes; ++i){
		if(newRoutes[i].size() > 0){
			// insert customers according to time windows' position
			vector<int> sorted;
			sorted.reserve(newRoutes[i].size());

			for(unsigned int j = 0; j < sorted.capacity(); ++j){
				int minStart = input[0].end, minEnd = input[0].end, id;
				for(unsigned int k = 0; k < newRoutes[i].size(); ++k){
					if(input[ newRoutes[i][k] ].end < minEnd){
						minEnd = input[ newRoutes[i][k] ].end;
						minStart = input[ newRoutes[i][k] ].start;
						id = k;
					}else if(input[ newRoutes[i][k] ].end == minEnd && input[ newRoutes[i][k] ].start < minStart ){
						minStart = input[ newRoutes[i][k] ].start;
						id = k;
					}
				}
				sorted.push_back(newRoutes[i][id]);
				newRoutes[i].erase(newRoutes[i].begin() + id);
			}
			
			route newRoute;
			newRoute.visits = list<int>(sorted.begin(), sorted.end());
			routes.push_back(newRoute);
		}
	}

	fitness(input);
}

void solution::fitness(const problem& input){
	totalDistance = totalTimewarp = 0;
	feasible = true;

	for(list<route>::iterator it = routes.begin(); it != routes.end(); ++it){
		it->fitness(input);
		totalDistance += it->distance;
		totalTimewarp += it->timewarp;
		if(it->feasible == false) feasible = false;
	}
}

int solution::cmp(const solution &solA, const solution &solB, const problem &input){
	if(solA.feasible != solB.feasible){
		if(solA.feasible) return -1;
		else return 1;
	}else{
		return (solA.routes.size() - solB.routes.size());
	}
}