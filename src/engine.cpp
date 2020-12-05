#include "engine.hpp"

#include "logger.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

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

MatchMap Engine::count_matches(const SetPtrEnsemble &sets)
{
  MatchMap matches;
  size_t   total_elements_to_process = 0;
  for (const auto &set : sets)
  {
    total_elements_to_process += set->size();
  }
  total_processed_ += total_elements_to_process;

  // A rough estimate of an average match-count operation for
  // large sets with dense value distribution is about half of
  // the total elements count.
  matches.reserve(total_elements_to_process / 2);

  for (const auto &set : sets)
  {
    for (const auto &element : *set)
    {
      ++matches[element];
    }
  }
  return matches;
}

SetPtr Engine::keep_matches_if(MatchMap &&matches, std::function<bool(size_t)> condition)
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
  total_processed_ += matches.size();
  return result;
}

size_t Engine::total_processed()
{
  return total_processed_;
}

SetPtr Engine::keep_if_less_than_n_matches(const SetPtrEnsemble &sets, int n)
{
  auto condition = [&n](size_t matches) { return matches < size_t(n); };
  return keep_matches_if(count_matches(sets), condition);
}

SetPtr Engine::keep_if_precisely_n_matches(const SetPtrEnsemble &sets, int n)
{
  auto condition = [&n](size_t matches) { return matches == size_t(n); };
  return keep_matches_if(count_matches(sets), condition);
}

SetPtr Engine::keep_if_greater_than_n_matches(const SetPtrEnsemble &sets, int n)
{
  auto condition = [&n](size_t matches) { return matches > size_t(n); };
  return keep_matches_if(count_matches(sets), condition);
}

SetPtr Engine::sets_intersection(const SetPtrEnsemble &sets)
{
  // Intersection of the N sets are all elements which occurs in
  // each set, e.g. N times.
  return keep_if_precisely_n_matches(sets, int(sets.size()));
}

SetPtr Engine::sets_difference(const SetPtrEnsemble &sets)
{
  // Difference of N sets are all the elements which occurs exactly once.
  return keep_if_precisely_n_matches(sets, 1);
}

SetPtr Engine::sets_union(const SetPtrEnsemble &sets)
{
  // Union of N sets contains all elements which occurs at least once.
  return keep_if_greater_than_n_matches(sets, 0);
}

SetPtr Engine::read_file(const std::string filename)
{
  std::ifstream ifs;
  ifs.open(filename, std::ifstream::in);
  if (!ifs.is_open())
  {
    throw std::runtime_error("can not open '" + filename + "', nothing to process.");
  }
  auto result = std::make_shared<Set>();

  DataType value = std::numeric_limits<DataType>::min();
  while (ifs >> value)
  {
    result->insert(value);
  }
  total_processed_ += result->size();
  return result;
}
