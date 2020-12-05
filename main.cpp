#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>

#include "engine.hpp"
#include "expression.hpp"

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
    // user_input = "[ SUM [ DIF a.txt b.txt c.txt ] [ INT b.txt c.txt ] ]";
    user_input = "[ GR 1 [ EQ 3 a.txt a.txt b.txt ] [ LE 2 b.txt c.txt ] ]";
    std::cout << "Please provide 'l' for explicit logging as first argument." << std::endl;
    std::cout << "Example expression: " << user_input << std::endl;
  }

  try
  {
    Expression expression = Expression::buildFromUserInput(user_input);

    auto start = std::chrono::system_clock::now();
    const auto result = expression.evaluate();
    std::vector<DataType> output;
    output.reserve(result.size());
    std::copy(result.begin(), result.end(), std::back_inserter(output));
    std::sort(output.begin(), output.end());
    auto end = std::chrono::system_clock::now();

    std::cout << "Result of size " << result.size() << ", processed total "
              << Engine::total_processed() << " elements in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " milliseconds:\n\n";

    Helpers::printVectorToCout(output);
  }
  catch (std::exception &e)
  {
    std::cout << "Error : " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
