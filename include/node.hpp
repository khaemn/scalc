#pragma once

#include "ops.hpp"
#include "types.hpp"

class Node
{
public:
  using NodeWeakPtr = std::weak_ptr<Node>;

  explicit Node(OpPtr operation, std::string name = "");

  ~Node() = default;

  SetPtrEnsemble gatherInputs() const;
  SetPtr evaluate();

  void addInput(NodeWeakPtr const &i);

  std::string const &         name() const;
  OperationType               operationType() const;

private:
  std::vector<NodeWeakPtr> input_nodes_;
  OpPtr       op_ptr_;
  std::string name_;
};
