#include "node.hpp"

#include "ops.hpp"

Node::Node(OpPtr operation, std::string name)
  : op_ptr_(operation)
  , name_(std::move(name))
{}

/**
 * returns a vector of all nodes which provide input to this node
 * @return vector of reference_wrapped tensors
 */
SetPtrEnsemble Node::gatherInputs() const
{
  SetPtrEnsemble inputs;
  for (auto const &i : input_nodes_)
  {
    if (auto ptr = i.lock())
    {
      inputs.push_back(ptr->evaluate());
    }
    else
    {
      throw std::runtime_error("Unable to lock weak pointer.");
    }
  }
  return inputs;
}

/**
 * Returns the result of evaluation of this node.
 * @return the set with the forward result
 */
SetPtr Node::evaluate()
{
  return op_ptr_->execute(gatherInputs());
}

const std::string &Node::name() const
{
  return name_;
}

OperationType Node::operationType() const
{
  return op_ptr_->type();
}

/**
 * registers a node as an input to this node
 * @param i pointer to the input node
 */
void Node::addInput(NodeWeakPtr const &i)
{
  input_nodes_.push_back(i);
}
