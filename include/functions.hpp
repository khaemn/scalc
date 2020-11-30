#pragma once

#include "ops.hpp"
#include "types.hpp"

VectorPtr naive_intersection(const InputData &sets);
VectorPtr naive_difference(const InputData &sets);
VectorPtr naive_union(const InputData &sets);

namespace Helpers {

void printVector(DataVector const &set);
void printVectorInLine(DataVector const &set);

}  // namespace Helpers
