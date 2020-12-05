#include "ops.hpp"

#include "functions.hpp"
#include "logger.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>

const std::map<OperationType, std::string> OP_NAMES{
    {OperationType::DIFFERENCE, "DIFFERENCE"},
    {OperationType::UNION, "UNION"},
    {OperationType::INTERSECTION, "INTERSECTION"},
    {OperationType::KEEP_IF_PRECISELY_N_MATCHES, "KEEP_IF_PRECISELY_N_MATCHES"},
    {OperationType::KEEP_IF_MORE_THAN_N_MATCHES, "KEEP_IF_MORE_THAN_N_MATCHES"},
    {OperationType::KEEP_IF_LESS_THAN_N_MATCHES, "KEEP_IF_LESS_THAN_N_MATCHES"},
    {OperationType::FILEREADER, "FILEREADER"},
    {OperationType::INTEGER, "INTEGER"},
    {OperationType::CONST_VECTOR, "CONST_VECTOR"},
    {OperationType::INVALID, "INVALID"},
    {OperationType::DUMMY, "DUMMY"}};

void validateTypeIsIn(OperationType type, const std::set<OperationType> allowed_types = {})
{
  if (allowed_types.find(type) == allowed_types.end())
  {
    throw std::runtime_error("Can not build operation: uncompatible type " + OP_NAMES.at(type));
  }
}

OpPtr buildOperation(OperationType type)
{
  switch (type)
  {
  case OperationType::DIFFERENCE:
    return std::static_pointer_cast<IOperation>(std::make_shared<OpDifference>());
  case OperationType::UNION:
    return std::static_pointer_cast<IOperation>(std::make_shared<OpUnion>());
  case OperationType::INTERSECTION:
    return std::static_pointer_cast<IOperation>(std::make_shared<OpIntersection>());
  default:
    throw std::runtime_error("Can not build operation of type " + OP_NAMES.at(type) +
                             " without an argument or parameter.");
  }
}

OpPtr buildOperation(OperationType type, std::string const &filename)
{
  validateTypeIsIn(type, {OperationType::FILEREADER});
  return std::static_pointer_cast<IOperation>(std::make_shared<OpFileReader>(filename));
}

OpPtr buildOperation(OperationType type, Set const &data)
{
  validateTypeIsIn(type, {OperationType::CONST_VECTOR});
  return std::static_pointer_cast<IOperation>(std::make_shared<OpHardcoded>(data));
}

OpPtr buildOperation(OperationType type, int parameter)
{
  switch (type)
  {
  case OperationType::KEEP_IF_PRECISELY_N_MATCHES:
    return std::static_pointer_cast<IOperation>(
        std::make_shared<OpKeepIfPreciselyNMatches>(parameter));
  case OperationType::KEEP_IF_MORE_THAN_N_MATCHES:
    return std::static_pointer_cast<IOperation>(
        std::make_shared<OpKeepIfMoreThanNMatches>(parameter));
  case OperationType::KEEP_IF_LESS_THAN_N_MATCHES:
    return std::static_pointer_cast<IOperation>(
        std::make_shared<OpKeepIfLessThanNMatches>(parameter));
  default:
    break;
  }
  throw std::runtime_error("Can not build operation of type " + OP_NAMES.at(type) +
                           " with an integer parameter.");
}

OpDifference::OpDifference()
  : IOperation(OperationType::DIFFERENCE)
{}

SetPtr OpDifference::evaluate(const SetPtrEnsemble &inputs)
{
  return sets_difference(inputs);
}

OpIntersection::OpIntersection()
  : IOperation(OperationType::INTERSECTION)
{}

SetPtr OpIntersection::evaluate(const SetPtrEnsemble &inputs)
{
  return sets_intersection(inputs);
}

OpUnion::OpUnion()
  : IOperation(OperationType::UNION)
{}

SetPtr OpUnion::evaluate(const SetPtrEnsemble &inputs)
{
  return sets_union(inputs);
}

OpFileReader::OpFileReader(const std::string &filename)
  : IOperation(OperationType::FILEREADER)
  , filename_(filename)
{}

extern size_t TOTAL_PROCESSED_ELEMENTS;

SetPtr OpFileReader::evaluate(const SetPtrEnsemble &)
{
  if (cache_)
  {
    return cache_;
  }
  std::ifstream ifs;
  ifs.open(filename_, std::ifstream::in);
  if (!ifs.is_open())
  {
    throw std::runtime_error("can not open '" + filename_ + "', nothing to process.");
  }
  auto result = std::make_shared<Set>();

  int prev_value = std::numeric_limits<int>::min();
  int value      = std::numeric_limits<int>::min();
  while (ifs >> value)
  {
    if (prev_value > value)
    {
      throw std::runtime_error("file '" + filename_ +
                               "' contains unsorted data, further processing results can be "
                               "invalid.");
    }
    prev_value = value;
    result->insert(value);
  }
  cache_ = result;
  TOTAL_PROCESSED_ELEMENTS += result->size();
  return result;
}

OpHardcoded::OpHardcoded(const Set &data)
  : IOperation(OperationType::CONST_VECTOR)
  , data_(data)
{}

SetPtr OpHardcoded::evaluate(const SetPtrEnsemble &)
{
  return std::make_shared<Set>(data_);
}

OpKeepIfMoreThanNMatches::OpKeepIfMoreThanNMatches(int parameter)
  : IOperation(OperationType::KEEP_IF_MORE_THAN_N_MATCHES)
  , parameter_(parameter)
{}

SetPtr OpKeepIfMoreThanNMatches::evaluate(const SetPtrEnsemble &inputs)
{
  return keep_if_greater_than_n_matches(inputs, parameter_);
}

OpKeepIfLessThanNMatches::OpKeepIfLessThanNMatches(int parameter)
  : IOperation(OperationType::KEEP_IF_LESS_THAN_N_MATCHES)
  , parameter_(parameter)
{}

SetPtr OpKeepIfLessThanNMatches::evaluate(const SetPtrEnsemble &inputs)
{
  return keep_if_less_than_n_matches(inputs, parameter_);
}

OpKeepIfPreciselyNMatches::OpKeepIfPreciselyNMatches(int parameter)
  : IOperation(OperationType::KEEP_IF_PRECISELY_N_MATCHES)
  , parameter_(parameter)
{}

SetPtr OpKeepIfPreciselyNMatches::evaluate(const SetPtrEnsemble &inputs)
{
  return keep_if_precisely_n_matches(inputs, parameter_);
}
