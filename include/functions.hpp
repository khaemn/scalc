#pragma once

#include "ops.hpp"
#include "types.hpp"

SetPtr keep_if_less_than_n_matches(const SetPtrEnsemble &sets, int n);
SetPtr keep_if_precisely_n_matches(const SetPtrEnsemble &sets, int n);
SetPtr keep_if_greater_than_n_matches(const SetPtrEnsemble &sets, int n);

SetPtr sets_intersection(const SetPtrEnsemble &sets);
SetPtr sets_difference(const SetPtrEnsemble &sets);
SetPtr sets_union(const SetPtrEnsemble &sets);

MatchMap count_matches(const SetPtrEnsemble &sets);

namespace Helpers {

void printVectorToCout(const std::vector<DataType> &vec);
void printVectorInLine(Set const &set);

}  // namespace Helpers
