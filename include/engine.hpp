#pragma once

#include "ops.hpp"
#include "types.hpp"

class Engine
{
public:
  static SetPtr keep_if_less_than_n_matches(const SetPtrEnsemble &sets, int n);
  static SetPtr keep_if_precisely_n_matches(const SetPtrEnsemble &sets, int n);
  static SetPtr keep_if_greater_than_n_matches(const SetPtrEnsemble &sets, int n);

  static SetPtr sets_intersection(const SetPtrEnsemble &sets);
  static SetPtr sets_difference(const SetPtrEnsemble &sets);
  static SetPtr sets_union(const SetPtrEnsemble &sets);

  static SetPtr read_file(const std::string filename);

  static size_t total_processed();

private:
  static MatchMap count_matches(const SetPtrEnsemble &sets);
  static SetPtr   keep_matches_if(MatchMap &&matches, std::function<bool(size_t)> condition);

  static size_t total_processed_;
};
namespace Helpers {

void printVectorToCout(const std::vector<DataType> &vec);
void printVectorInLine(Set const &set);

}  // namespace Helpers
