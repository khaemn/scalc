#include "node.hpp"
#include "ops.hpp"

Node::Node(OpPtr operation, std::string name)
  : cached_output_(new Set)
  , op_ptr_(operation)
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
 * Returns the result of a forward evaluation of this node. If that's already been
 * computed this is cheap; if not then Forward is called as necessary if the output
 * size has not been updated since last used. This also must be changed and
 * recalculated as necessary
 * @return the tensor with the forward result
 */
SetPtr Node::evaluate()
{
  if (cached_output_->empty())
  {
    SetPtrEnsemble inputs = gatherInputs();

    cached_output_ = op_ptr_->evaluate(inputs);
  }

  return cached_output_;
}

/**
 * Resets input and output node ptr containers. Useful for graph decompiling.
 */
void Node::resetInputsAndOutputs()
{
  input_nodes_.clear();
  outputs_.clear();
}

const std::string &Node::name() const
{
  return name_;
}

std::shared_ptr<IOperation> Node::getOp()
{
  return op_ptr_;
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

/**
 * registers a node as an output to this node
 * @param o pointer to the output node
 */
void Node::addOutput(NodeWeakPtr const &o)
{
  outputs_.push_back(o);
}
