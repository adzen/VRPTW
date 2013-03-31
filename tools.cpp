#include <cstdio>

#include "tools.h"

using namespace std;

bool loadData(const char* filename, vector<customer> &customers, int &capacity){
	FILE *fp = fopen(filename, "r");

	if(fp == NULL) return false;

	int inputCapacity;
	if( fscanf(fp, "%d", &inputCapacity) != 1 ){
		fclose(fp);
		return false;
	}
	capacity = inputCapacity;

	int inputNum;
	if( fscanf(fp, "%d", &inputNum) != 1 ){
		fclose(fp);
		return false;
	}
	customers.reserve(inputNum + 1);

	for(int i = 0; i <= inputNum; ++i){
		double x, y;
		int d, s, e, u;

		if( fscanf(fp, "%*d%lf%lf%d%d%d%d", &x, &y, &d, &s, &e, &u) != 6 ){
			fclose(fp);
			return false;
		}

		customer newCustomer(x, y, d, s, e, u);
		customers.push_back(newCustomer);
	}

	fclose(fp);
	return true;
}