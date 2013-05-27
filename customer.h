#ifndef CUSTOMER_H
#define CUSTOMER_H

class customer{
public:
	customer(int id, double x, double y, int demand, int start, int end, int unload);
	
	int id;
	double x_pos, y_pos;
	int demand, start, end, unload;
};

#endif