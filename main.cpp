#include <iostream>
#include <string>

#include "functions.hpp"
#include "graph.hpp"

int main(int argc, char **argv)
{
  std::string user_input;

  if (argc > 1)
  {
    int first_expression_arg_index = 1;
    if (argv[1][0] == 'l')
    {
      Logger::instance().setEnabled(true);
      ++first_expression_arg_index;
    }
    for (int argnum{first_expression_arg_index}; argnum < argc; ++argnum)
    {
      user_input.append(std::string(argv[argnum]) + " ");
    }
  }
  else
  {
    user_input = "[ SUM [ DIF a.txt b.txt c.txt ] [ INT b.txt c.txt ] ]";
    std::cout << "Please provide 'l' for explicit logging as first argument." << std::endl;
    std::cout << "Example expression: " << user_input << std::endl;
  }

  try
  {
    Graph expression = Graph::buildFromUserInput(user_input);
    Helpers::printVector(expression.evaluate());
  }
  catch (std::exception &e)
  {
    std::cout << "Error : " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
