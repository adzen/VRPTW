#ifndef CUSTOMER_H
#define CUSTOMER_H

class customer{
public:
	customer(double x, double y, int demand, int start, int end, int unload);
	
	const double x_pos, y_pos;
	const int demand, start, end, unload;
};

#endif