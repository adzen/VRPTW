#ifndef TOOLS_H
#define TOOLS_H

#include <vector>

#include "customer.h"

bool loadData(const char* filename, std::vector<customer> &customers, int &capacity);

#endif