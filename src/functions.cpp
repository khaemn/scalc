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
  auto processor = [](DataVector const &v1, DataVector const &v2, DataVector &out) {
    std::set_intersection(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), std::back_inserter(out));
  };
  return process_inputs_by_pair(sets, processor);
}

VectorPtr naive_difference(const InputData &sets)
{
  auto processor = [](DataVector const &v1, DataVector const &v2, DataVector &out) {
    std::set_difference(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), std::back_inserter(out));
  };
  return process_inputs_by_pair(sets, processor);
}

VectorPtr naive_union(const InputData &sets)
{
  auto processor = [](DataVector const &v1, DataVector const &v2, DataVector &out) {
    std::set_union(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), std::back_inserter(out));
  };
  return process_inputs_by_pair(sets, processor);
}

namespace Helpers {

void printVector(const DataVector &set)
{
  for (auto value : set)
  {
    std::cout << value << std::endl;
  }
}

void printVectorInLine(const DataVector &set)
{
  for (auto value : set)
  {
    std::cout << value << " ";
  }
  std::cout << std::endl;
}

}  // namespace Helpers
