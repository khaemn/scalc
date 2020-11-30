#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "types.hpp"

class Lexer
{
public:
  static std::vector<Token> parseUserInput(std::string const &input);

  static void printTokens(std::vector<Token> const &tokens);

  static Token parseSubstring(std::string const &substring);

  static const std::map<std::string, Lexem> FIXED_LEXEM_NAMES;
  static const std::set<Lexem> PARAMETRIZED_LEXEMS;
  static bool isParametrized(Lexem lexem);
};
