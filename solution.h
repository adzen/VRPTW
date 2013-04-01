#ifndef SOLUTION_H
#define SOLUTION_H

#include <cstdio>

#include <list>

#include "route.h"

class solution{
public:
	void print(FILE *fp) const;
	void clear();
	void random(int totalCustomers, int maxRoutes);

	std::list<route> routes;
	double totalDistance;
};

#endif