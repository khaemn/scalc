#pragma once

#include <map>
#include <string>
#include <vector>

#include "types.hpp"

class Lexer
{
public:
  static std::vector<Token> getTokensFrom(std::string const &input);

  static void printTokens(std::vector<Token> const &tokens);

  static Token parseSubstring(std::string const &substring);

  static const std::map<std::string, Lexem> FIXED_LEXEMS;
};
