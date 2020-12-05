#include "expression.hpp"

#include "lexer.hpp"

#include <iostream>
#include <limits>
#include <set>
#include <vector>

static constexpr auto PARAM_SEPARATOR{"|"};

Expression::Expression(IEngine &engine)
  : engine_(engine)
{}

void Expression::buildFromUserInput(const std::string &input)
{
  if (is_compiled_)
  {
    throw std::runtime_error("Attempt to build an Expression which is already built.");
  }
  std::vector<Token> tokens = Lexer::parseUserInput(input);
  buildFromTokens(tokens);
}

void Expression::buildFromTokens(std::vector<Token> const &tokens)
{
  if (is_compiled_)
  {
    throw std::runtime_error("Attempt to build an Expression which is already built.");
  }
  Logger &log{Logger::instance()};
  using NodePtr                = Expression::NodePtrType;
  using NodeBuffer             = std::vector<NodePtr>;
  using NodeMatrix             = std::vector<NodeBuffer>;
  static constexpr auto INDENT = "   ";

  std::vector<Token> stack;
  stack.reserve(tokens.size());

  size_t node_counter = 0;

  NodeMatrix unlinked_nodes;
  size_t     depth = 0;
  log << "Parsing expression : "
      << "\n";
  Lexer::printTokens(tokens);
  log << "\n";

  for (size_t token_idx{0}; token_idx < tokens.size(); ++token_idx)
  {
    const Token &token = tokens.at(token_idx);
    Lexer::printTokens(stack);
    log << INDENT << ">" << token.value << "<"
        << "\n";

    if (token.lexem != Lexem::CLOSE)
    {
      if (Lexer::isParametrized(token.lexem))
      {
        // The next token is SPACE, and after it we expect a NUMBER
        // token which contains the actual parameter of an LE, GE, EQ etc. operation.
        const auto &parameter = tokens.at(token_idx + 1);
        if (parameter.lexem != Lexem::INTEGER)
        {
          log << "Can not parse " << parameter.value << " as an integer parameter of "
              << token.value;
          throw std::runtime_error("Parsing failed");
        }
        Token parametrized = {token.lexem, token.value + PARAM_SEPARATOR + parameter.value};
        stack.push_back(parametrized);

        ++token_idx;  // skip the parameter lexem
      }
      else
      {
        stack.push_back(token);
      }

      if (token.lexem == Lexem::OPEN)
      {
        ++depth;
        log << "Expression depth changed : " << depth << "\n";
        if (unlinked_nodes.size() < depth)
        {
          unlinked_nodes.emplace_back(NodeBuffer{});
        }
      }
      continue;
    }

    // If a "]" encountered, go back in stack until "[" found and create all nodes,
    // then bind the leftmost one to all other ones and save this leftmost node as a
    // parent node into arg-buffer for this depth level.
    if (stack.size() < 2)
    {
      throw std::runtime_error("Parser stack underflow, probably input is incorrect");
    }
    log << "Expression block end found, parsing the block ... "
        << "\n";
    NodeBuffer new_nodes;
    while (stack.back().lexem != Lexem::OPEN)
    {
      auto op = buildOperationFromToken(stack.back());

      // Every node should have a unique name except for file readers: only one filereader
      // per one filename is allowed to prevent duplicating of huge file caches.
      std::string node_name =
          op->description() + "_" + stack.back().value +
          (op->type() == OperationType::FILEREADER ? "" : ("_" + std::to_string(node_counter)));

      if (this->getNode(node_name).get())
      {
        log << "A node with name " << node_name << " already exists in the expression.\n";
        new_nodes.push_back(this->getNode(node_name));
      }
      else
      {
        new_nodes.emplace_back(std::make_shared<Node>(op, node_name));
        this->insertNode(node_name, new_nodes.back());
        log << "  Created node " << node_name << "\n";
        ++node_counter;
      }
      stack.pop_back();
    }
    stack.pop_back();  // removing "["

    NodePtr parent = new_nodes.back();
    new_nodes.pop_back();

    if (new_nodes.empty())
    {
      // If no new nodes were created while parsing this block, it means all included blocks are
      // already processed and the only thing left is to link them together and leave the block.
      auto &ready_nodes = unlinked_nodes[depth];
      if (ready_nodes.empty())
      {
        log << "    !---Error: no ready nodes on depth " << depth << " to combine!"
            << "\n";
      }
      while (!ready_nodes.empty())
      {
        auto const &child = ready_nodes.back();
        parent->addInput(Node::NodeWeakPtr(child));
        ready_nodes.pop_back();

        log << INDENT << parent->name() << "--connect-input-to-->" << child->name() << "\n";
      }
      log << INDENT << "All nodes in a block on depth " << depth
          << " linked successfuly, parent node added to buffer on depth " << depth - 1 << "\n";
      unlinked_nodes[depth - 1].push_back(parent);
    }
    else
    {
      // If there are new nodes, created during parsing this block, they must be added to expression
      // expression.
      while (!new_nodes.empty())
      {
        auto const &child = new_nodes.back();
        log << INDENT << parent->name() << "--connect-input-to-->" << child->name() << "\n";
        parent->addInput(Node::NodeWeakPtr(child));
        new_nodes.pop_back();
      }
      auto &ready_nodes = unlinked_nodes[depth - 1];
      // arg_buffers.pop_back();
      ready_nodes.push_back(parent);
      log << INDENT << "Pushed a parent node from depth " << depth << " to " << depth - 1 << "\n";
      log << INDENT << "Arg buffer size " << ready_nodes.size() << " depth " << depth
          << " total bufs " << unlinked_nodes.size() << "\n";
    }
    --depth;  // we are done on this depth level probably?
    log << "Expression depth changed : " << depth << "\n";
  }

  if (stack.empty())
  {
    log << "Parsing finished successfuly, created a Graph with " << node_counter << " nodes."
        << "\n";

    this->setOutputNodeName(unlinked_nodes.front().back()->name());
    this->compile();
  }
  else
  {
    throw std::runtime_error("Input parsing failed! Unparsed/invalid input lexem : " +
                             stack.back().value);
  }
}

/**
 * @brief A fabric method for construction of a necessary Operation according to lexem.
 * @param token
 * @return shared pointer to a constructed Op.
 */
OpPtr Expression::buildOperationFromToken(Token const &token)
{
  switch (token.lexem)
  {
  case Lexem::FILENAME:
    return buildOperation(engine_, OperationType::FILEREADER, token.value);
  case Lexem::INT:
    return buildOperation(engine_, OperationType::INTERSECTION);
  case Lexem::DIFF:
    return buildOperation(engine_, OperationType::DIFFERENCE);
  case Lexem::SUM:
    return buildOperation(engine_, OperationType::UNION);
  case Lexem::EQ:
    return buildOperation(engine_, OperationType::KEEP_IF_PRECISELY_N_MATCHES,
                          std::stoi(token.value.substr(token.value.find(PARAM_SEPARATOR) + 1)));
  case Lexem::LE:
    return buildOperation(engine_, OperationType::KEEP_IF_LESS_THAN_N_MATCHES,
                          std::stoi(token.value.substr(token.value.find(PARAM_SEPARATOR) + 1)));
  case Lexem::GR:
    return buildOperation(engine_, OperationType::KEEP_IF_MORE_THAN_N_MATCHES,
                          std::stoi(token.value.substr(token.value.find(PARAM_SEPARATOR) + 1)));
  default: {
    Logger &log_{Logger::instance()};
    log_ << "Error: unknown token ( " << token.value
         << " ) encountered while building an Operation."
         << "\n";
    return OpPtr{};
  }
  }
}

/**
 * Uses the connections object to link together inputs to nodes
 * Having a separate compile stage allows for arbitrary order of addNode calls
 */
void Expression::compile()
{
  // set inputs and outputs to nodes and set trainables
  for (auto &connection : connections_)
  {
    auto node_name   = connection.first;
    auto node_inputs = connection.second;
    linkNodesInGraph(node_name, node_inputs);
  }
  is_compiled_ = true;
}

/**
 * Evaluates the output of a node (calling all necessary evaluations)
 * @param node_name name of node to evaluate for output
 * @return a copy of the output tensor
 */
Set Expression::evaluate(std::string const &node_name)
{
  if (nodes_.find(node_name) == nodes_.end())
  {
    throw std::runtime_error("Cannot evaluate: node [" + node_name + "] not in graph");
  }
  auto ret = (*(nodes_[node_name]->evaluate()));
  return ret;
}

/**
 * @brief A wrapper for more convenient evaluation call
 * @return
 */
Set Expression::evaluate()
{
  return evaluate(outputNodeName());
}

/**
 * Method for directly inserting nodes to graph
 * @param node_name
 * @return false if insertion failed
 */
bool Expression::insertNode(std::string const &node_name, NodePtrType node_ptr)
{
  // put node in look up table
  nodes_[node_name] = node_ptr;
  return nodes_.find(node_name) != nodes_.end();
}

/**
 * Method for getting a ptr to a graph node by its name.
 * @param node_name
 * @return nullptr if there is no such node.
 */
Expression::NodePtrType Expression::getNode(const std::string &node_name)
{
  if (nodes_.find(node_name) != nodes_.cend())
  {
    return nodes_.at(node_name);
  }
  return NodePtrType{nullptr};
}

/**
 * Connect the new node to the current graph by setting input and output nodes to it and saving it
 * in the lookup table.
 * @param node_name
 * @param inputs
 */
void Expression::linkNodesInGraph(std::string const &             node_name,
                                  std::vector<std::string> const &inputs)
{
  for (auto const &i : inputs)
  {
    nodes_.at(node_name)->addInput(nodes_.at(i));
  }
}

std::string Expression::outputNodeName() const
{
  return output_node_name_;
}

void Expression::setOutputNodeName(const std::string &outputNodeName)
{
  output_node_name_ = outputNodeName;
}
