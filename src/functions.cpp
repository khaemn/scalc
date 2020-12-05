#include "functions.hpp"

#include <algorithm>
#include <iostream>
#include <list>

#include "logger.hpp"

#define NO_INPUT_VECTOR_VALIDATION

static bool validate_input(const InputData &sets)
{
#ifndef NO_INPUT_VECTOR_VALIDATION
  for (auto const &set : sets)
  {
    if (!std::is_sorted(set->begin(), set->end()))
    {
      Logger::getInstance() << "Error: one or more input set(s) not sorted.";
      return false;
    }
  }

  if (sets.empty())
  {
    Logger::getInstance() << "Error: nothing to process.";
    return false;
  }
#endif  // NO_INPUT_VECTOR_VALIDATION
  return true;
}

static VectorPtr process_inputs_by_pair(
    const InputData &sets,
    std::function<void(DataVector const &v1, DataVector const &v2, DataVector &out)>
        processing_call)
{
  if (!validate_input(sets))
  {
    Logger::instance() << "Error: invalid input detected, can not process."
                          << "\n";
    return std::make_shared<DataVector>();
  }

  if (sets.size() == 1)
  {
    return sets.front();
  }

  std::list<VectorPtr> processing_queue;

  for (VectorPtr const &vector_ptr : sets)
  {
    processing_queue.push_back(vector_ptr);
  }

  while (processing_queue.size() > 1)
  {
    // TODO: parallelize queue processing
    VectorPtr v1 = processing_queue.front();
    processing_queue.pop_front();
    VectorPtr v2 = processing_queue.front();
    processing_queue.pop_front();
    VectorPtr merged = std::make_shared<DataVector>();

    processing_call(*v1, *v2, *merged);
    processing_queue.push_front(merged);
  }

  return processing_queue.front();
}

VectorPtr naive_intersection(const InputData &sets)
{
//  auto processor = [](DataVector const &v1, DataVector const &v2, DataVector &out) {
//    std::set_intersection(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), std::back_inserter(out));
//  };
//  return process_inputs_by_pair(sets, processor);
    // DEBUG!
    return {sets.back()};
}

VectorPtr naive_difference(const InputData &sets)
{
//  auto processor = [](DataVector const &v1, DataVector const &v2, DataVector &out) {
//    std::set_difference(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), std::back_inserter(out));
//  };
//  return process_inputs_by_pair(sets, processor);
    // DEBUG!
    return {sets.back()};
}

VectorPtr naive_union(const InputData &sets)
{
//  auto processor = [](DataVector const &v1, DataVector const &v2, DataVector &out) {
//    std::set_union(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), std::back_inserter(out));
//  };
//  return process_inputs_by_pair(sets, processor);
    // DEBUG!
    return {sets.back()};
}

namespace Helpers {

void printVectorToCout(const std::vector<DataType> &vec)
{
  for (auto value : vec)
  {
    std::cout << value << std::endl;
  }
}

void printVectorInLine(const DataVector &set)
{
  for (auto value : set)
  {
    Logger::instance() << value << " ";
  }
}

}  // namespace Helpers

extern size_t TOTAL_PROCESSED_ELEMENTS;
MatchMap count_matches_simplified(const InputData &sets)
{
  MatchMap matches;
  size_t output_size_rough_prediction = 0;
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

MatchMap count_matches(const InputData &sets)
{
  auto &   log = Logger::instance();
  MatchMap matches;

  log << "--- Input sets:---------\n";
  for (const auto& s : sets) {
      Helpers::printVectorInLine(*s);
  }
  log << "------------------------\n";

  // TODO: a good point for parallelization.
  auto       current = sets.begin();
  auto       other   = sets.begin();
  const auto end     = sets.end();
  while (current != end)
  {
    for (const auto &element : *(*current))
    {
      if (matches.find(element) != matches.end())
      {
        // If an element already exists in the map, it means that
        // at some of the previous iterations all matches for it
        // have been already found.
        continue;
      }
      matches[element] += 1;
      while (other != end)
      {
        if (other != current)
        {
          //log << "Searching for " << element << " in "; Helpers::printVectorInLine(*(*other));
          //log << std::endl;
          // DEBUG! if (std::binary_search((*other)->begin(), (*other)->end(), element))
          if ((*other)->count(element))
          {
            //  log << "Match!\n";
            matches[element] += 1;
          }
        }
        other++;
      }
      other = sets.begin();
    }
    current++;
    other = sets.begin();
  }
//  log << "===== Match map : ===\n";
//  for (const auto m : matches)
//  {
//    log << m.first << " : " << m.second << " \n";
//  }
//  log << "=====================\n";
  return matches;
}

VectorPtr keep_matches_if(MatchMap &&matches, std::function<bool(size_t)> condition)
{
  auto result = std::make_shared<DataVector>();
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

VectorPtr keep_if_less_than_n_matches(const InputData &sets, int n)
{
  auto condition = [&n](size_t matches) { return matches < size_t(n); };
  return keep_matches_if(count_matches_simplified(sets), condition);
}

VectorPtr keep_if_precisely_n_matches(const InputData &sets, int n)
{
  auto condition = [&n](size_t matches) { return matches == size_t(n); };
  return keep_matches_if(count_matches_simplified(sets), condition);
}

VectorPtr keep_if_greater_than_n_matches(const InputData &sets, int n)
{
  auto condition = [&n](size_t matches) { return matches > size_t(n); };
  return keep_matches_if(count_matches_simplified(sets), condition);
}

VectorPtr sets_intersection(const InputData &sets)
{
  // Intersection of the N sets are all elements which occurs in
  // each set, e.g. N times.
  return keep_if_precisely_n_matches(sets, int(sets.size()));
}

VectorPtr sets_difference(const InputData &sets)
{
  // Difference of N sets are all the elements which occurs exactly once.
  return keep_if_precisely_n_matches(sets, 1);
}

VectorPtr sets_union(const InputData &sets)
{
  // Union of N sets contains all elements which occurs at least once.
  return keep_if_greater_than_n_matches(sets, 0);
}
