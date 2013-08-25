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
	totalDistance = totalTimewarp = totalWaiting = unbalancedCapacity = exceededCapacity = 0;
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
			newRoute.modified = true;
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
		if(newRoute.load == 0){
			// if the newRoute is empty, insert this customer
			newRoute.visits.push_back(ids[i]);
			
			if(input.getDistance(0, ids[i]) < input[ ids[i] ].start){
				newRoute.waiting += input[ ids[i] ].start - input.getDistance(0, ids[i]);
				finish = input[ ids[i] ].start;
			}else{
				finish = input.getDistance(0, ids[i]);
			}
			finish += input[ ids[i] ].unload;

			newRoute.load += input[ ids[i] ].demand;
			newRoute.distance += input.getDistance(0, ids[i]);
		}else{
			// if the newRoute isn't empty...
			if(newRoute.load + input[ ids[i] ].demand <= input.getCapacity() &&
				finish + input.getDistance(newRoute.visits.back(), ids[i]) <= input[ ids[i] ].end){
				// will not overload & can arrive before the deadline
				double arrival = finish + input.getDistance(newRoute.visits.back(), ids[i]);
				double start = (arrival < input[ ids[i] ].start) ? (input[ ids[i] ].start) : (arrival);
				
				// after the insertion, if we can still back to depot before the deadline
				if(start + input[ ids[i] ].unload + input.getDistance(ids[i], 0) <= input[0].end){
					newRoute.waiting += (arrival < input[ ids[i] ].start) ? (input[ ids[i] ].start - arrival) : (0);
					newRoute.distance += input.getDistance(newRoute.visits.back(), ids[i]);
					newRoute.visits.push_back(ids[i]);
					finish = start + input[ ids[i] ].unload;
					newRoute.load += input[ ids[i] ].demand;
				}
			}
		}

		// if this customer still can't be appended, start a new route is needed
		if(newRoute.visits.back() != ids[i]){
			newRoute.distance += input.getDistance(newRoute.visits.back(), 0);
			newRoute.feasible = true;
			newRoute.modified = false;
			routes.push_back(newRoute);
			totalDistance += newRoute.distance;
			totalWaiting += newRoute.waiting;
			unbalancedCapacity += (input.getCapacity() - newRoute.load);

			newRoute.clear();
			finish = 0;

			i--;
		}
	}

	if(newRoute.load != 0){
		newRoute.distance += input.getDistance(newRoute.visits.back(), 0);
		newRoute.feasible = true;
		newRoute.modified = false;
		routes.push_back(newRoute);
		totalDistance += newRoute.distance;
		totalWaiting += newRoute.waiting;
		unbalancedCapacity += (input.getCapacity() - newRoute.load);
	}

	// just for testing...
	// double dis = totalDistance;
	// fitness(input);
	// if(totalDistance != dis || totalTimewarp > 0 || !feasible) puts("ERROR!");
}

// Solomon's I1 insertion heuristic (1987)
// Ref.: "Algorithms for the Vehicle Routing and Scheduling Problems with Time Window Constraints"
void solution::solomon(const problem& input, bool farthest, double mu, double lambda, double alpha1){
	clear();

	// initialize
	vector<int> unrouted;
	unrouted.resize(input.getNumCusto());
	for(int id = 1; id <= input.getNumCusto(); id++) unrouted[id-1] = id;

	while( !unrouted.empty() ){
		route r;

		// choose first customer
		unsigned int index;
		if(farthest){  // the farthest unrouted customer
			double farthest = 0;
			for(unsigned int c = 0; c < unrouted.size(); c++){
				if(input.getDistance(0, unrouted[c]) > farthest){
					index = c;
					farthest = input.getDistance(0, unrouted[c]);
				}
			}
		}else{  // the unrouted customer with the earliest deadline
			int earliest = input[0].end;
			for(unsigned int c = 0; c < unrouted.size(); c++){
				if(input[ unrouted[c] ].end < earliest){
					index = c;
					earliest = input[ unrouted[c] ].end;
				}
			}
		}
		r.visits.push_front(unrouted[index]);
		unrouted.erase(unrouted.begin() + index);


	
		routes.push_front(r);
	}

	fitness(input);
}

void solution::fitness(const problem& input){
	totalDistance = totalTimewarp = totalWaiting = unbalancedCapacity = exceededCapacity = 0;
	feasible = true;

	for(list<route>::iterator it = routes.begin(); it != routes.end(); ++it){
		it->fitness(input);
		totalDistance += it->distance;
		totalTimewarp += it->timewarp;
		totalWaiting += it->waiting;
		unbalancedCapacity += (it->load > input.getCapacity()) ? 
			(it->load - input.getCapacity()) : (input.getCapacity() - it->load);
		if(it->load > input.getCapacity() ) exceededCapacity += (it->load - input.getCapacity());
		if(it->feasible == false) feasible = false;
	}
}

int solution::cmp(const solution &solA, const solution &solB, const problem &input){
	if(solA.feasible != solB.feasible){
		if(solA.feasible) return -1;
		else return 1;
	}else if(solA.feasible){
		if(solA.routes.size() != solB.routes.size() ) return (solA.routes.size() - solB.routes.size() );
		else if(solA.totalDistance < solB.totalDistance) return -1;
		else return 1;
	}else{
		if(solA.totalTimewarp < solB.totalTimewarp) return -1;
		else return 1;
	}
}

bool solution::fdominate(const solution &solA, const solution &solB){
	if( solA.totalDistance <= solB.totalDistance && solA.routes.size() <= solB.routes.size() ){
		return ( solA.totalDistance < solB.totalDistance || solA.routes.size() < solB.routes.size() );
	}else return false;
}

bool solution::idominate(const solution &solA, const solution &solB){
	if( solA.exceededCapacity <= solB.exceededCapacity && solA.routes.size() <= solB.routes.size() && solA.totalTimewarp <= solB.totalTimewarp ){
		return ( solA.exceededCapacity < solB.exceededCapacity || solA.routes.size() < solB.routes.size() || solA.totalTimewarp < solB.totalTimewarp );
	}else return false;
}

bool solution::operator == (const solution &another) const {
	return (routes.size() == another.routes.size() && 
			unbalancedCapacity == another.unbalancedCapacity &&
            fabs(totalDistance - another.totalDistance) < 0.01);
}

bool solution::operator < (const solution &another) const {
	if(routes.size() != another.routes.size() ) return (routes.size() < another.routes.size());
	else return (totalDistance < another.totalDistance);
}