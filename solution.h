#ifndef SOLUTION_H
#define SOLUTION_H

#include <cstdio>
#include <list>
#include <vector>
#include "customer.h"
#include "route.h"
#include "problem.h"

class solution{
public:
	void print(FILE *fp) const;
	void clear();
	void random(int maxRoutes, const problem& input);
	void fitness(const problem& input);

	std::list<route> routes;
	double totalDistance, totalTimewarp;
	bool feasible;
};

#endif