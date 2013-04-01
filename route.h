#ifndef ROUTE_H
#define ROUTE_H

#include <cstdio>

#include <list>

class route{
public:
	void print(FILE *fp) const;

	std::list<int> customers;
	double distance;
};

#endif