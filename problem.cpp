#include "problem.h"

bool problem::load(const char* filename){
	FILE *fp = fopen(filename, "r");

	if(fp == NULL) return false;

	if( fscanf(fp, "%d", &capacity) != 1 ){
		fclose(fp);
		return false;
	}

	int inputNum;
	if( fscanf(fp, "%d", &inputNum) != 1 ){
		fclose(fp);
		return false;
	}
	allCustomer.reserve(inputNum + 1);    // with depot

	for(int i = 0; i <= inputNum; ++i){
		double x, y;
		int d, s, e, u;

		if( fscanf(fp, "%*d%lf%lf%d%d%d%d", &x, &y, &d, &s, &e, &u) != 6 ){
			fclose(fp);
			return false;
		}

		customer newCustomer(x, y, d, s, e, u);
		allCustomer.push_back(newCustomer);
	}
	fclose(fp);
	
	calDistances();
	return true;
}

int problem::getCapacity() const {
	return capacity;
}

int problem::getNumCusto() const {
	return allCustomer.size() - 1;    // ignore depot 
}

double problem::getDistance(int id1, int id2) const {
	return distance[id1][id2];
}

const customer& problem::operator [] (int id) const {
	return allCustomer[id];
}

// minimum # of route = ceil(total demand / vehicle's capacity);
int problem::calMinRoute() const {
	int totalDemand = getCapacity() - 1;    // ceiling
	for(int i = 1; i <= getNumCusto(); ++i) totalDemand += allCustomer[i].demand;
	return totalDemand / getCapacity();
}

void problem::calDistances(){
	distance.resize( allCustomer.size() );

	for(unsigned int i = 0; i < allCustomer.size(); ++i){
		distance[i].resize( allCustomer.size() );
	}

	for(unsigned int x = 0;   x < allCustomer.size(); ++x){
	for(unsigned int y = x+1; y < allCustomer.size(); ++y){
		double ans = hypot(allCustomer[x].x_pos - allCustomer[y].x_pos,
						   allCustomer[x].y_pos - allCustomer[y].y_pos);
		distance[x][y] = distance[y][x] = ans;
	}}
}
