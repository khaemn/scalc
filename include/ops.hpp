#pragma once

#include "types.hpp"

#include <functional>
#include <map>
#include <memory>
#include <vector>

class IEngine;

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
  TOTAL_OP_TYPES
};

extern const std::map<OperationType, std::string> OP_NAMES;

class Operation
{
public:
  Operation(IEngine &engine, OperationType type)
    : type_(type)
    , engine_(engine)
  {}
  virtual ~Operation() = default;

  virtual std::shared_ptr<Set> execute(SetPtrEnsemble const &inputs) = 0;

  virtual OperationType type() const
  {
    return type_;
  }
  virtual std::string description() const;

protected:
  OperationType type_ = OperationType::INVALID;
  IEngine &     engine_;
};

using OpPtr = std::shared_ptr<Operation>;

class OpDifference : public Operation
{
public:
  explicit OpDifference(IEngine &engine);
  SetPtr execute(const SetPtrEnsemble &inputs) override;
};

class OpIntersection : public Operation
{
public:
  explicit OpIntersection(IEngine &engine);
  SetPtr execute(const SetPtrEnsemble &inputs) override;
};

class OpUnion : public Operation
{
public:
  explicit OpUnion(IEngine &engine);
  SetPtr execute(const SetPtrEnsemble &inputs) override;
};

class OpFileReader : public Operation
{
public:
  explicit OpFileReader(IEngine &engine, std::string const &filename);
  ~OpFileReader() override = default;
  SetPtr execute(const SetPtrEnsemble &) override;

private:
  std::string filename_;
  SetPtr      cache_{nullptr};
};

class OpHardcoded : public Operation
{
public:
  explicit OpHardcoded(IEngine &engine, Set const &data);
  ~OpHardcoded() override = default;
  SetPtr execute(const SetPtrEnsemble &inputs) override;

private:
  Set data_;
};

class OpKeepIfMoreThanNMatches : public Operation
{
public:
  explicit OpKeepIfMoreThanNMatches(IEngine &engine, int parameter);
  SetPtr execute(const SetPtrEnsemble &inputs) override;

private:
  int parameter_;
};

class OpKeepIfLessThanNMatches : public Operation
{
public:
  explicit OpKeepIfLessThanNMatches(IEngine &engine, int parameter);
  SetPtr execute(const SetPtrEnsemble &inputs) override;

private:
  int parameter_;
};

class OpKeepIfPreciselyNMatches : public Operation
{
public:
  explicit OpKeepIfPreciselyNMatches(IEngine &engine, int parameter);
  SetPtr execute(const SetPtrEnsemble &inputs) override;

private:
  int parameter_;
};

/// A family of standalone fabrics to produce a necessary Operation depending on itsy type and
/// arguments.
OpPtr buildOperation(IEngine &engine, OperationType type);
OpPtr buildOperation(IEngine &engine, OperationType type, const std::string &filename);
OpPtr buildOperation(IEngine &engine, OperationType type, Set const &data);
OpPtr buildOperation(IEngine &engine, OperationType type, int parameter);
