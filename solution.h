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
	void random(const problem& input);
	void fitness(const problem& input);
	static int cmp(const solution &solA, const solution &solB, const problem &input);
	static bool dominate(const solution &solA, const solution &solB);

	std::list<route> routes;
	double totalDistance, totalTimewarp, totalWaiting;
	int unbalancedCapacity;
	bool feasible;
};

#endif