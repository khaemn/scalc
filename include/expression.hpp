#pragma once

#include "engine.hpp"
#include "logger.hpp"
#include "node.hpp"

#include <map>
#include <vector>

class IEngine;

class Expression
{
public:
  using NodePtrType = std::shared_ptr<Node>;

  explicit Expression(IEngine& engine);
  void buildFromUserInput(std::string const &input);
  void buildFromTokens(std::vector<Token> const &tokens);

  template <OperationType op_type, typename... Params>
  Node::NodeWeakPtr addNode(std::string const &node_name, std::vector<std::string> const &inputs,
                            Params... params);

  Set evaluate(std::string const &node_name);
  Set evaluate();

  bool        insertNode(std::string const &node_name, NodePtrType node_ptr);
  NodePtrType getNode(std::string const &node_name);
  bool        contains(std::string const &node_name) const;

  std::string outputNodeName() const;
  void        setOutputNodeName(const std::string &outputNodeName);

protected:
  std::map<std::string, NodePtrType>                            nodes_;
  std::vector<std::pair<std::string, std::vector<std::string>>> connections_;

private:
  OpPtr buildOperationFromToken(Token const &token);

  void compile();
  void linkNodesInGraph(std::string const &node_name, std::vector<std::string> const &inputs);

  IEngine& engine_;
  std::string output_node_name_{};
  bool is_compiled_{false};

  Logger &log_{Logger::instance()};
};

/**
 * the interface for adding nodes to the expression.
 * The parameters are node name, names of input nodes, and any op parameters
 * @tparam OperationType Type of Op node will compute
 * @tparam Params parameters for the Op construction
 * @param node_name name of the new node to add
 * @param inputs names of other nodes that feed input to this node
 * @param params input parameters for the op construction
 * @return the updated name of the node that was added. this may differ from that specified by the
 * user
 */
template <OperationType op_type, typename... Params>
Node::NodeWeakPtr Expression::addNode(std::string const &             node_name,
                                      std::vector<std::string> const &inputs, Params... params)
{
  NodePtrType node_ptr;

  // Instantiate the node based on params
  OpPtr op = buildOperation(op_type, params...);
  node_ptr = std::make_shared<Node>(op, node_name);

  // put node in look up table
  nodes_[node_name] = node_ptr;

  // define connections between nodes
  connections_.emplace_back(std::make_pair(node_name, inputs));

  return Node::NodeWeakPtr(node_ptr);
}
