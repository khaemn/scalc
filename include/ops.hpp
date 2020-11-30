#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "types.hpp"

enum class OperationType
{
  DIFFERENCE,
  UNION,
  INTERSECTION,

  EQUAL_NUM_OF_MATCHES,
  GREATER_NUM_OF_MATCHES,
  LESS_NUM_OF_MATCHES,

  FILEREADER,
  INTEGER,
  INVALID,
  DUMMY,
};

class IOperation
{
public:
  IOperation()          = default;
  virtual ~IOperation() = default;

  virtual std::shared_ptr<DataVector> evaluate(InputData const &inputs) = 0;

  virtual OperationType type() const        = 0;
  virtual std::string   description() const = 0;
};

using OpPtr = std::shared_ptr<IOperation>;

class OpDifference : public IOperation
{
public:
  VectorPtr evaluate(const InputData &inputs) override;

  OperationType      type() const override;
  inline std::string description() const override;
};

class OpIntersection : public IOperation
{
public:
  VectorPtr          evaluate(const InputData &inputs) override;
  OperationType      type() const override;
  inline std::string description() const override;
};

class OpUnion : public IOperation
{
public:
  VectorPtr          evaluate(const InputData &inputs) override;
  OperationType      type() const override;
  inline std::string description() const override;
};

class OpFileReader : public IOperation
{
public:
  explicit OpFileReader(std::string const &filename);
  ~OpFileReader() override = default;

  VectorPtr evaluate(const InputData &) override;

  OperationType      type() const override;
  inline std::string description() const override;

private:
  std::string filename_;
  VectorPtr   cache_{};
};

class OpHardcoded : public IOperation
{
public:
  explicit OpHardcoded(DataVector const &data);
  ~OpHardcoded() override = default;

  VectorPtr evaluate(const InputData &inputs) override;

  OperationType      type() const override;
  inline std::string description() const override;

private:
  DataVector data_;
};

/// A family of standalone fabrics to produce a necessary Operation depending on itsy type and
/// arguments.
OpPtr buildOperation(OperationType type);
OpPtr buildOperation(OperationType type, const std::string &filename);
OpPtr buildOperation(OperationType type, DataVector const &data);
