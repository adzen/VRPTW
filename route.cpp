#include "route.h"

using namespace std;

void route::print(FILE *fp) const {
	fprintf(fp, "%d customers, distance = %.2f :", customers.size(), distance);
	
	for(list<int>::const_iterator it = customers.begin(); it != customers.end(); ++it){
		fprintf(fp, " %d", *it);
	}

	fprintf(fp, "\n");
}
