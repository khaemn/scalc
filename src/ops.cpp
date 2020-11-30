#include "ops.hpp"
#include "functions.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

OpFileReader::OpFileReader(const std::string &filename)
  : filename_(filename)
{}

std::string OpFileReader::description() const
{
  return "File_Reader_";
}

OpHardcoded::OpHardcoded(const DataVector &data)
  : data_(data)
{}

std::string OpHardcoded::description() const
{
  return "Hardcoded_Data_";
}

std::string OpUnion::description() const
{
  return "Set_Union_";
}

std::string OpIntersection::description() const
{
  return "Set_Intersection_";
}

std::string OpDifference::description() const
{
  return "Set_Difference_";
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
    throw std::runtime_error("Can not build opeartion of unknown type.");
  }
}

OpPtr buildOperation(OperationType type, std::string const &filename)
{
  if (type != OperationType::FILEREADER)
  {
    throw std::runtime_error("Invalid attempt to build node!");
  }
  return std::static_pointer_cast<IOperation>(std::make_shared<OpFileReader>(filename));
}

OpPtr buildOperation(OperationType type, DataVector const &data)
{
  if (type != OperationType::HARDCODED_INPUT)
  {
    throw std::runtime_error("Invalid attempt to build node!");
  }
  return std::static_pointer_cast<IOperation>(std::make_shared<OpHardcoded>(data));
}

VectorPtr OpDifference::evaluate(const InputData &inputs)
{
  return naive_difference(inputs);
}

OperationType OpDifference::type() const
{
  return OperationType::DIFFERENCE;
}

VectorPtr OpIntersection::evaluate(const InputData &inputs)
{
  return naive_intersection(inputs);
}

OperationType OpIntersection::type() const
{
  return OperationType::INTERSECTION;
}

VectorPtr OpUnion::evaluate(const InputData &inputs)
{
  return naive_union(inputs);
}

OperationType OpUnion::type() const
{
  return OperationType::UNION;
}

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

OperationType OpFileReader::type() const
{
  return OperationType::FILEREADER;
}

VectorPtr OpHardcoded::evaluate(const InputData &)
{
  return std::make_shared<DataVector>(data_);
}

OperationType OpHardcoded::type() const
{
  return OperationType::HARDCODED_INPUT;
}
