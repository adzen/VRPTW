#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include "customer.h"

class problem{
public:
	bool load(const char* filename);
	int getCapacity() const;
	int getNumCusto() const;
	double getDistance(int id1, int id2) const;
	const customer& operator [] (int id) const;
	int calMinRoute() const;

private:
	void calDistances();

	std::vector<customer> allCustomer;
	std::vector< std::vector<double> > distance;
	int capacity;
	bool shortHorizon;
};

#endif