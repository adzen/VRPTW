#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <list>
#include "problem.h"
#include "solution.h"

solution crossover(const solution &a, const solution &b, const problem &input);
const solution& tournament(const std::list<solution> &population, const problem &input);

void ranking(const std::list<solution> &population, std::vector< std::list<solution> > *output);
void environmental(const std::vector< std::list<solution> > &ranked, std::list<solution> *output, unsigned int maxSize);

#endif