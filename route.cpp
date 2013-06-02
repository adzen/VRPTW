#include <algorithm>

#include "route.h"

using namespace std;

void route::print(FILE *fp) const {
	fprintf(fp, "%d customers, distance = %.3f :", visits.size(), distance);
	
	for(list<int>::const_iterator it = visits.begin(); it != visits.end(); ++it){
		fprintf(fp, " %d", *it);
	}

	fprintf(fp, "\n");
}

void route::clear(){
	visits.clear();
	distance = timewarp = waiting = load = 0;
}

void route::fitness(const problem& input){
	vector<int> ids(visits.begin(), visits.end() );
	
	// reset all attributes
	distance = timewarp = waiting = load = 0;
	feasible = true;

	double arrival = input.getDistance(0, ids[0]);
	distance = arrival;
	for(unsigned int i = 0; i < ids.size(); ++i){
		load += input[ ids[i] ].demand;
		if( load > input.getCapacity() ) feasible = false;
	
		if(arrival > input[ ids[i] ].end){    // late arrival: time-wrap
			feasible = false;
			timewarp += arrival - input[ ids[i] ].end;
			arrival = input[ ids[i] ].end;
		}else if(arrival < input[ ids[i] ].start){    // early arrival: wait
			waiting += input[ ids[i] ].start - arrival;
			arrival = input[ ids[i] ].start;
		}

		// unload
		arrival += input[ ids[i] ].unload;

		// traveling
		if( i == ids.size() - 1 ){  // back to depot
			arrival += input.getDistance(ids[i], 0);
			distance += input.getDistance(ids[i], 0);
		}else{  // goto next customer
			arrival += input.getDistance(ids[i], ids[i+1]);
			distance += input.getDistance(ids[i], ids[i+1]);
		}
	}

	if(arrival > input[0].end){
		feasible = false;
		timewarp += arrival - input[0].end;
	}
}

// further improvement
int route::cmp(const route& routeA, const route& routeB, const problem& input){
	if(routeA.feasible != routeB.feasible){
		if(routeA.feasible) return -1;
		else return 1;
	}else{
		if(routeA.feasible){
			return (int)(routeA.distance - routeB.distance);
		}else{
			if(routeA.load < input.getCapacity()) return -1;
			if(routeB.load < input.getCapacity()) return 1;

			if(routeA.timewarp == 0.0) return -1;
			if(routeB.timewarp == 0.0) return 1;

			return 0;
		}
	}
}

bool route::hasCus(int cusID) const {
	return ( find(visits.begin(), visits.end(), cusID) != visits.end() );
}