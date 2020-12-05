#pragma once

#include "ops.hpp"
#include "types.hpp"

class IEngine
{
public:
  IEngine()                                                                        = default;
  virtual ~IEngine()                                                               = default;
  virtual SetPtr keep_if_less_than_n_matches(const SetPtrEnsemble &sets, int n)    = 0;
  virtual SetPtr keep_if_precisely_n_matches(const SetPtrEnsemble &sets, int n)    = 0;
  virtual SetPtr keep_if_greater_than_n_matches(const SetPtrEnsemble &sets, int n) = 0;

  virtual SetPtr sets_intersection(const SetPtrEnsemble &sets) = 0;
  virtual SetPtr sets_difference(const SetPtrEnsemble &sets)   = 0;
  virtual SetPtr sets_union(const SetPtrEnsemble &sets)        = 0;

  virtual SetPtr read_file(const std::string filename) = 0;
};

class Engine : public IEngine
{
public:
  SetPtr keep_if_less_than_n_matches(const SetPtrEnsemble &sets, int n) override;
  SetPtr keep_if_precisely_n_matches(const SetPtrEnsemble &sets, int n) override;
  SetPtr keep_if_greater_than_n_matches(const SetPtrEnsemble &sets, int n) override;

  SetPtr sets_intersection(const SetPtrEnsemble &sets) override;
  SetPtr sets_difference(const SetPtrEnsemble &sets) override;
  SetPtr sets_union(const SetPtrEnsemble &sets) override;

  SetPtr read_file(const std::string filename) override;

  size_t total_processed();

private:
  MatchMap count_matches(const SetPtrEnsemble &sets);
  SetPtr   keep_matches_if(MatchMap &&matches, std::function<bool(size_t)> condition);

  size_t total_processed_{0};
};

namespace Helpers {

void printVectorToCout(const std::vector<DataType> &vec);
void printVectorInLine(Set const &set);

}  // namespace Helpers
