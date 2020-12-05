#pragma once

#include "ops.hpp"
#include "types.hpp"

VectorPtr naive_intersection(const InputData &sets);
VectorPtr naive_difference(const InputData &sets);
VectorPtr naive_union(const InputData &sets);

VectorPtr sets_intersection(const InputData &sets);
VectorPtr sets_difference(const InputData &sets);
VectorPtr sets_union(const InputData &sets);

VectorPtr keep_if_less_than_n_matches(const InputData &sets, int n);
VectorPtr keep_if_precisely_n_matches(const InputData &sets, int n);
VectorPtr keep_if_greater_than_n_matches(const InputData &sets, int n);

MatchMap count_matches(const InputData &sets);

namespace Helpers {

void printVectorToCout(const std::vector<DataType> &vec);
void printVectorInLine(DataVector const &set);

}  // namespace Helpers
