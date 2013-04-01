#include <vector>
#include <algorithm>

#include "solution.h"

using namespace std;

void solution::print(FILE *fp) const {
	fprintf(fp, "%d routes, distance = %.2f\n", routes.size(), totalDistance);
	for(list<route>::const_iterator it = routes.begin(); it != routes.end(); ++it){
		it->print(fp);
	}
}

void solution::clear(){
	routes.clear();
	totalDistance = 0;
}

void solution::random(int totalCustomers, int maxRoutes){
	solution::clear();

	vector< vector<int> > newRoutes;
	newRoutes.resize(maxRoutes);

	for(int id = 1; id <= totalCustomers; ++id){
		newRoutes[ rand() % maxRoutes ].push_back(id);
	}

	for(int i = 0; i < maxRoutes; ++i){
		if(newRoutes[i].size() > 0){
			random_shuffle(newRoutes[i].begin(), newRoutes[i].end());
			
			route newRoute;
			newRoute.distance = 0;
			newRoute.customers = list<int>(newRoutes[i].begin(), newRoutes[i].end());
			routes.push_back(newRoute);
		}
	}
}