#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <map>

#include "types.hpp"

enum class OperationType
{
  DIFFERENCE,
  UNION,
  INTERSECTION,

  KEEP_IF_PRECISELY_N_MATCHES,
  KEEP_IF_MORE_THAN_N_MATCHES,
  KEEP_IF_LESS_THAN_N_MATCHES,

  FILEREADER,
  INTEGER,
  CONST_VECTOR,
  INVALID,
  DUMMY,
};

extern const std::map<OperationType, std::string> OP_NAMES;

class IOperation
{
public:
  IOperation(OperationType type)
    : type_(type)
  {}
  virtual ~IOperation() = default;

  virtual std::shared_ptr<Set> evaluate(SetPtrEnsemble const &inputs) = 0;

  virtual OperationType type() const
  {
    return type_;
  }
  virtual std::string description() const
  {
    return OP_NAMES.at(type());
  }

protected:
  OperationType type_ = OperationType::INVALID;
};

using OpPtr = std::shared_ptr<IOperation>;

class OpDifference : public IOperation
{
public:
  explicit OpDifference();
  SetPtr evaluate(const SetPtrEnsemble &inputs) override;
};

class OpIntersection : public IOperation
{
public:
  explicit OpIntersection();
  SetPtr evaluate(const SetPtrEnsemble &inputs) override;
};

class OpUnion : public IOperation
{
public:
  explicit OpUnion();
  SetPtr evaluate(const SetPtrEnsemble &inputs) override;
};

class OpFileReader : public IOperation
{
public:
  explicit OpFileReader(std::string const &filename);
  ~OpFileReader() override = default;
  SetPtr evaluate(const SetPtrEnsemble &) override;

private:
  std::string filename_;
  SetPtr   cache_{};
};

class OpHardcoded : public IOperation
{
public:
  explicit OpHardcoded(Set const &data);
  ~OpHardcoded() override = default;
  SetPtr evaluate(const SetPtrEnsemble &inputs) override;

private:
  Set data_;
};

class OpKeepIfMoreThanNMatches : public IOperation
{
public:
  explicit OpKeepIfMoreThanNMatches(int parameter);
  SetPtr evaluate(const SetPtrEnsemble &inputs) override;

private:
  int parameter_;
};

class OpKeepIfLessThanNMatches : public IOperation
{
public:
  explicit OpKeepIfLessThanNMatches(int parameter);
  SetPtr evaluate(const SetPtrEnsemble &inputs) override;

private:
  int parameter_;
};

class OpKeepIfPreciselyNMatches : public IOperation
{
public:
  explicit OpKeepIfPreciselyNMatches(int parameter);
  SetPtr evaluate(const SetPtrEnsemble &inputs) override;

private:
  int parameter_;
};

/// A family of standalone fabrics to produce a necessary Operation depending on itsy type and
/// arguments.
OpPtr buildOperation(OperationType type);
OpPtr buildOperation(OperationType type, const std::string &filename);
OpPtr buildOperation(OperationType type, Set const &data);
OpPtr buildOperation(OperationType type, int parameter);

