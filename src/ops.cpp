#include "ops.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>

#include "functions.hpp"
#include "logger.hpp"

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

OpPtr buildOperation(OperationType type, DataVector const &data)
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

VectorPtr OpDifference::evaluate(const InputData &inputs)
{
    return naive_difference(inputs);
}

OpIntersection::OpIntersection()
  : IOperation(OperationType::INTERSECTION)
{}

VectorPtr OpIntersection::evaluate(const InputData &inputs)
{
    return naive_intersection(inputs);
}

OpUnion::OpUnion()
  : IOperation(OperationType::UNION)
{}

VectorPtr OpUnion::evaluate(const InputData &inputs)
{
  return naive_union(inputs);
}

OpFileReader::OpFileReader(const std::string &filename)
  : IOperation(OperationType::FILEREADER)
  , filename_(filename)
{}

VectorPtr OpFileReader::evaluate(const InputData &)
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
  auto result = std::make_shared<DataVector>();

  int prev_value = std::numeric_limits<int>::min();
  int value      = std::numeric_limits<int>::min();
  while (ifs >> value)
  {
    if (prev_value > value)
    {
      throw std::runtime_error("file '" + filename_ +
                               "' contains unsorted data, further processing results would be "
                               "invalid.");
    }
    prev_value = value;
    result->emplace_back(value);
  }
  cache_ = result;
  return result;
}

OpHardcoded::OpHardcoded(const DataVector &data)
  : IOperation(OperationType::CONST_VECTOR)
  , data_(data)
{}

VectorPtr OpHardcoded::evaluate(const InputData &)
{
  return std::make_shared<DataVector>(data_);
}

OpKeepIfMoreThanNMatches::OpKeepIfMoreThanNMatches(int parameter)
  : IOperation(OperationType::KEEP_IF_MORE_THAN_N_MATCHES)
  , parameter_(parameter)
{}

VectorPtr OpKeepIfMoreThanNMatches::evaluate(const InputData &inputs)
{
  throw std::runtime_error("OpKeepMoreThanNMatches NOT IMPLEMETED");
}

OpKeepIfLessThanNMatches::OpKeepIfLessThanNMatches(int parameter)
  : IOperation(OperationType::KEEP_IF_LESS_THAN_N_MATCHES)
  , parameter_(parameter)
{}

VectorPtr OpKeepIfLessThanNMatches::evaluate(const InputData &inputs)
{
  throw std::runtime_error("OpKeepIfLessThanNMatches NOT IMPLEMETED");
}

OpKeepIfPreciselyNMatches::OpKeepIfPreciselyNMatches(int parameter)
  : IOperation(OperationType::KEEP_IF_PRECISELY_N_MATCHES)
  , parameter_(parameter)
{}

VectorPtr OpKeepIfPreciselyNMatches::evaluate(const InputData &inputs)
{
  throw std::runtime_error("OpKeepIfPreciselyNMatches NOT IMPLEMETED");
}
