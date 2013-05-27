#include "customer.h"

customer::customer(int i, double x, double y, int d, int s, int e, int u) : 
	id(i), x_pos(x), y_pos(y), demand(d), start(s), end(e), unload(u) {
}