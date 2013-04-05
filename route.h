#ifndef ROUTE_H
#define ROUTE_H

#include <cstdio>
#include <list>
#include <vector>
#include "customer.h"
#include "problem.h"


class route{
public:
	void print(FILE *fp) const;
	void fitness(const problem& input);
	void sorting(const problem& input);

	std::list<int> visits;
	double distance, timewarp;
	int totalDemand;
	bool feasible;
};

#endif