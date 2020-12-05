#include "functions.hpp"

#include "logger.hpp"

#include <algorithm>
#include <iostream>
#include <list>

#define NO_INPUT_VECTOR_VALIDATION

namespace Helpers {

void printVectorToCout(const std::vector<DataType> &vec)
{
  for (auto value : vec)
  {
    std::cout << value << std::endl;
  }
}

void printVectorInLine(const Set &set)
{
  for (auto value : set)
  {
    Logger::instance() << value << " ";
  }
}

}  // namespace Helpers

extern size_t TOTAL_PROCESSED_ELEMENTS;
MatchMap      count_matches(const SetPtrEnsemble &sets)
{
  MatchMap matches;
  size_t   output_size_rough_prediction = 0;
  for (const auto &set : sets)
  {
    output_size_rough_prediction += set->size();
  }
  TOTAL_PROCESSED_ELEMENTS += output_size_rough_prediction;
  // A rough estimate of an average match-count operation for
  // large sets with dense value distribution is about half of
  // the total elements count.
  matches.reserve(output_size_rough_prediction / 2);

  for (const auto &set : sets)
  {
    for (const auto &element : *set)
    {
      matches[element] += 1;
    }
  }
  return matches;
}

SetPtr keep_matches_if(MatchMap &&matches, std::function<bool(size_t)> condition)
{
  auto result = std::make_shared<Set>();
  result->reserve(matches.size() / 2);
  for (const auto &match : matches)
  {
    if (condition(match.second))
    {
      result->insert(match.first);
    }
  }
  TOTAL_PROCESSED_ELEMENTS += matches.size();
  return result;
}

SetPtr keep_if_less_than_n_matches(const SetPtrEnsemble &sets, int n)
{
  auto condition = [&n](size_t matches) { return matches < size_t(n); };
  return keep_matches_if(count_matches(sets), condition);
}

SetPtr keep_if_precisely_n_matches(const SetPtrEnsemble &sets, int n)
{
  auto condition = [&n](size_t matches) { return matches == size_t(n); };
  return keep_matches_if(count_matches(sets), condition);
}

SetPtr keep_if_greater_than_n_matches(const SetPtrEnsemble &sets, int n)
{
  auto condition = [&n](size_t matches) { return matches > size_t(n); };
  return keep_matches_if(count_matches(sets), condition);
}

SetPtr sets_intersection(const SetPtrEnsemble &sets)
{
  // Intersection of the N sets are all elements which occurs in
  // each set, e.g. N times.
  return keep_if_precisely_n_matches(sets, int(sets.size()));
}

SetPtr sets_difference(const SetPtrEnsemble &sets)
{
  // Difference of N sets are all the elements which occurs exactly once.
  return keep_if_precisely_n_matches(sets, 1);
}

SetPtr sets_union(const SetPtrEnsemble &sets)
{
  // Union of N sets contains all elements which occurs at least once.
  return keep_if_greater_than_n_matches(sets, 0);
}
