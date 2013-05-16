#include <algorithm>

#include "solution.h"

using namespace std;

void solution::print(FILE *fp) const {
	fprintf(fp, "[%d routes, distance = %.3f, timewarp = %.3f, unbalance = %d, %s]\n",
		routes.size(), totalDistance, totalTimewarp, unbalancedCapacity, (feasible) ? "feasible" : "infeasible");
	
	for(list<route>::const_iterator it = routes.begin(); it != routes.end(); ++it){
		it->print(fp);
	}
}

void solution::clear(){
	routes.clear();
	totalDistance = totalTimewarp = unbalancedCapacity = 0;
}

void solution::random(int maxRoutes, const problem& input){
	clear();


	vector< vector<int> > newRoutes;
	newRoutes.resize(maxRoutes);

	// insert all customers into routes randomly
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
				// find unrouted customer with smallest end-time, then smallest start-time
				for(unsigned int k = 0; k < newRoutes[i].size(); ++k){
					if(input[ newRoutes[i][k] ].end < minEnd){
						minEnd = input[ newRoutes[i][k] ].end;
						minStart = input[ newRoutes[i][k] ].start;
						id = k;
					}else if(input[ newRoutes[i][k] ].end == minEnd &&
							 input[ newRoutes[i][k] ].start < minStart ){
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

void solution::random(const problem& input){
	// random shuffle all customer's id
	vector<int> ids(input.getNumCusto());
	for(unsigned int i = 0; i < ids.size(); ++i) ids[i] = i+1;
	random_shuffle( ids.begin(), ids.end() );

	clear();
	route newRoute;
	newRoute.clear();
	double finish = 0;
	for(unsigned int i = 0; i < ids.size(); ++i){
		if(newRoute.totalDemand == 0){
			// if the newRoute is empty, insert this customer
			newRoute.visits.push_back(ids[i]);
			
			if(input.getDistance(0, ids[i]) < input[ ids[i] ].start){
				finish = input[ ids[i] ].start;
			}else{
				finish = input.getDistance(0, ids[i]);
			}
			finish += input[ ids[i] ].unload;

			newRoute.totalDemand += input[ ids[i] ].demand;
			newRoute.distance += input.getDistance(0, ids[i]);
		}else{
			// if the newRoute isn't empty...
			if(newRoute.totalDemand + input[ ids[i] ].demand <= input.getCapacity() &&
				finish + input.getDistance(newRoute.visits.back(), ids[i]) <= input[ ids[i] ].end){
				// will not overload & can arrive before the deadline
				double arrival = finish + input.getDistance(newRoute.visits.back(), ids[i]);
				double start = (arrival < input[ ids[i] ].start) ? (input[ ids[i] ].start) : (arrival);
				
				// after the insertion, if we can still back to depot before the deadline
				if(start + input[ ids[i] ].unload + input.getDistance(ids[i], 0) <= input[0].end){
					newRoute.distance += input.getDistance(newRoute.visits.back(), ids[i]);
					newRoute.visits.push_back(ids[i]);
					finish = start + input[ ids[i] ].unload;
					newRoute.totalDemand += input[ ids[i] ].demand;
				}
			}
		}

		// if this customer still can't be appended, start a new route is needed
		if(newRoute.visits.back() != ids[i]){
			newRoute.distance += input.getDistance(newRoute.visits.back(), 0);
			newRoute.timewarp = 0;
			newRoute.feasible = true;
			routes.push_back(newRoute);
			totalDistance += newRoute.distance;
			unbalancedCapacity += (newRoute.totalDemand > input.getCapacity()) ? 
				(newRoute.totalDemand - input.getCapacity()) : (input.getCapacity() - newRoute.totalDemand);

			newRoute.clear();
			finish = 0;

			i--;
		}
	}

	if(newRoute.totalDemand != 0){
		newRoute.distance += input.getDistance(newRoute.visits.back(), 0);
		newRoute.timewarp = 0;
		newRoute.feasible = true;
		routes.push_back(newRoute);
		totalDistance += newRoute.distance;
		unbalancedCapacity += (newRoute.totalDemand > input.getCapacity()) ? 
			(newRoute.totalDemand - input.getCapacity()) : (input.getCapacity() - newRoute.totalDemand);
	}

	// just for testing...
	// double dis = totalDistance;
	// fitness(input);
	// if(totalDistance != dis || totalTimewarp > 0 || !feasible) puts("ERROR!");
}

void solution::fitness(const problem& input){
	totalDistance = totalTimewarp = unbalancedCapacity = 0;
	feasible = true;

	for(list<route>::iterator it = routes.begin(); it != routes.end(); ++it){
		it->fitness(input);
		totalDistance += it->distance;
		totalTimewarp += it->timewarp;
		unbalancedCapacity += (it->totalDemand > input.getCapacity()) ? 
			(it->totalDemand - input.getCapacity()) : (input.getCapacity() - it->totalDemand);
		if(it->feasible == false) feasible = false;
	}
}

int solution::cmp(const solution &solA, const solution &solB, const problem &input){
	if(solA.feasible != solB.feasible){
		if(solA.feasible) return -1;
		else return 1;
	}else if(solA.feasible){
		return (solA.unbalancedCapacity - solB.unbalancedCapacity);
	}else{
		if(solA.totalTimewarp < solB.totalTimewarp) return -1;
		else return 1;
	}
}

bool solution::dominate(const solution &solA, const solution &solB){
	if( solA.totalDistance <= solB.totalDistance && solA.routes.size() <= solB.routes.size() ){
		return ( solA.totalDistance < solB.totalDistance || solA.routes.size() < solB.routes.size() );
	}else return false;
}