#include "ops.hpp"

#include "engine.hpp"
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
    {OperationType::INVALID, "INVALID"}};

void validateTypeIsIn(OperationType type, const std::set<OperationType> allowed_types = {})
{
  if (allowed_types.find(type) == allowed_types.end())
  {
    throw std::runtime_error("Can not build operation: uncompatible type " + OP_NAMES.at(type));
  }
}

OpPtr buildOperation(IEngine &engine, OperationType type)
{
  switch (type)
  {
  case OperationType::DIFFERENCE:
    return std::static_pointer_cast<Operation>(std::make_shared<OpDifference>(engine));
  case OperationType::UNION:
    return std::static_pointer_cast<Operation>(std::make_shared<OpUnion>(engine));
  case OperationType::INTERSECTION:
    return std::static_pointer_cast<Operation>(std::make_shared<OpIntersection>(engine));
  default:
    throw std::runtime_error("Can not build operation of type " + OP_NAMES.at(type) +
                             " without an argument or parameter.");
  }
}

OpPtr buildOperation(IEngine &engine, OperationType type, std::string const &filename)
{
  validateTypeIsIn(type, {OperationType::FILEREADER});
  return std::static_pointer_cast<Operation>(std::make_shared<OpFileReader>(engine, filename));
}

OpPtr buildOperation(IEngine &engine, OperationType type, Set const &data)
{
  validateTypeIsIn(type, {OperationType::CONST_VECTOR});
  return std::static_pointer_cast<Operation>(std::make_shared<OpHardcoded>(engine, data));
}

OpPtr buildOperation(IEngine &engine, OperationType type, int parameter)
{
  switch (type)
  {
  case OperationType::KEEP_IF_PRECISELY_N_MATCHES:
    return std::static_pointer_cast<Operation>(
        std::make_shared<OpKeepIfPreciselyNMatches>(engine, parameter));
  case OperationType::KEEP_IF_MORE_THAN_N_MATCHES:
    return std::static_pointer_cast<Operation>(
        std::make_shared<OpKeepIfMoreThanNMatches>(engine, parameter));
  case OperationType::KEEP_IF_LESS_THAN_N_MATCHES:
    return std::static_pointer_cast<Operation>(
        std::make_shared<OpKeepIfLessThanNMatches>(engine, parameter));
  default:
    break;
  }
  throw std::runtime_error("Can not build operation of type " + OP_NAMES.at(type) +
                           " with an integer parameter.");
}

OpDifference::OpDifference(IEngine &engine)
  : Operation(engine, OperationType::DIFFERENCE)
{}

SetPtr OpDifference::execute(const SetPtrEnsemble &inputs)
{
  return engine_.sets_difference(inputs);
}

OpIntersection::OpIntersection(IEngine &engine)
  : Operation(engine, OperationType::INTERSECTION)
{}

SetPtr OpIntersection::execute(const SetPtrEnsemble &inputs)
{
  return engine_.sets_intersection(inputs);
}

OpUnion::OpUnion(IEngine &engine)
  : Operation(engine, OperationType::UNION)
{}

SetPtr OpUnion::execute(const SetPtrEnsemble &inputs)
{
  return engine_.sets_union(inputs);
}

OpFileReader::OpFileReader(IEngine &engine, const std::string &filename)
  : Operation(engine, OperationType::FILEREADER)
  , filename_(filename)
{}

SetPtr OpFileReader::execute(const SetPtrEnsemble &)
{
  if (!cache_)
  {
    cache_ = engine_.read_file(filename_);
  }
  return cache_;
}

OpHardcoded::OpHardcoded(IEngine &engine, const Set &data)
  : Operation(engine, OperationType::CONST_VECTOR)
  , data_(data)
{}

SetPtr OpHardcoded::execute(const SetPtrEnsemble &)
{
  return std::make_shared<Set>(data_);
}

OpKeepIfMoreThanNMatches::OpKeepIfMoreThanNMatches(IEngine &engine, int parameter)
  : Operation(engine, OperationType::KEEP_IF_MORE_THAN_N_MATCHES)
  , parameter_(parameter)
{}

SetPtr OpKeepIfMoreThanNMatches::execute(const SetPtrEnsemble &inputs)
{
  return engine_.keep_if_greater_than_n_matches(inputs, parameter_);
}

OpKeepIfLessThanNMatches::OpKeepIfLessThanNMatches(IEngine &engine, int parameter)
  : Operation(engine, OperationType::KEEP_IF_LESS_THAN_N_MATCHES)
  , parameter_(parameter)
{}

SetPtr OpKeepIfLessThanNMatches::execute(const SetPtrEnsemble &inputs)
{
  return engine_.keep_if_less_than_n_matches(inputs, parameter_);
}

OpKeepIfPreciselyNMatches::OpKeepIfPreciselyNMatches(IEngine &engine, int parameter)
  : Operation(engine, OperationType::KEEP_IF_PRECISELY_N_MATCHES)
  , parameter_(parameter)
{}

SetPtr OpKeepIfPreciselyNMatches::execute(const SetPtrEnsemble &inputs)
{
  return engine_.keep_if_precisely_n_matches(inputs, parameter_);
}

std::string Operation::description() const
{
  return OP_NAMES.at(type());
}
