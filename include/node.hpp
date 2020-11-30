#pragma once

#include "ops.hpp"
#include "types.hpp"

class Node
{
public:
  using NodeWeakPtr = std::weak_ptr<Node>;

  explicit Node(OpPtr operation, std::string name = "");

  ~Node() = default;

  InputData gatherInputs() const;
  VectorPtr evaluate();

  void addInput(NodeWeakPtr const &i);
  void addOutput(NodeWeakPtr const &o);
  void resetInputsAndOutputs();

  std::string const &         name() const;
  std::shared_ptr<IOperation> getOp();
  OperationType               operationType() const;

private:
  std::vector<NodeWeakPtr> input_nodes_;
  std::vector<NodeWeakPtr> outputs_;

  VectorPtr cached_output_;

  OpPtr       op_ptr_;
  std::string name_;
};
