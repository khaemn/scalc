#include <iostream>
#include <string>
#include <chrono>

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
    // user_input = "[ SUM [ DIF a.txt b.txt c.txt ] [ INT b.txt c.txt ] ]";
    user_input = "[ GR 1 [ EQ 3 a.txt a.txt b.txt ] [ LE 2 b.txt c.txt ] ]";
    std::cout << "Please provide 'l' for explicit logging as first argument." << std::endl;
    std::cout << "Example expression: " << user_input << std::endl;
  }

  try
  {
    Graph expression = Graph::buildFromUserInput(user_input);

    auto start = std::chrono::system_clock::now();
    const auto result = expression.evaluate();
    auto end = std::chrono::system_clock::now();

    // EQ 10 of 10 files 1M elements in range (0..5M) each takes +- 32000 msec and
    // gives 906706 matches (~1M). Total elements to process 10M.
    // EQ 5 of 5 files 1M elements in range (0..5M) each takes +- 9500 msec and
    // gives 906706 matches (~1M). Total elements to process 5M.
    // EQ 3 of 3 files 1M elements in range (0..5M) each takes +- 5650 msec and
    // gives 906706 matches (~1M). Total elements to process 3M.
    // EQ 10 of 10 files 1k elements in range (0..5k) each takes +- 17 msec and
    // gives 913 matches (~1k). Total elements to process 10k
    // EQ 10 of 5 files  1k elements in range (0..5k) each takes 9 msec and
    // gives 0 results (expected)
    // EQ 5 of 5 files  1k elements in range (0..5k) each takes 9 msec and
    // gives 913 results (~1k). Total 5k elements
    std::cout << "Result in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " milliseconds:\n\n";
    Helpers::printVector(result);
  }
  catch (std::exception &e)
  {
    std::cout << "Error : " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
