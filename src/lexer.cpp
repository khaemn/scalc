#include "lexer.hpp"

#include <iostream>

#include "logger.hpp"

static constexpr std::size_t       MAX_ALLOWED_FILENAME_LENGTH = 255;
const std::map<std::string, Lexem> Lexer::FIXED_LEXEMS{
    {"[", Lexem::OPEN},
    {"]", Lexem::CLOSE},
    //
    {"SUM", Lexem::SUM},
    {"INT", Lexem::INT},
    {"DIF", Lexem::DIFF},
    //
    {" ", Lexem::SPACE},
};

// A filename must contain a dot
static constexpr char   FILENAME_SIGN               = '.';
static constexpr char   SPACE                       = ' ';
static constexpr size_t MAX_LINE_WIDTH_FOR_PRINTING = 42;

/**
 * @brief Performs lexical analysis of a user input string.
 * @param input
 * @return
 */
std::vector<Token> Lexer::getTokensFrom(const std::string &input)
{
  Logger::instance() << "Starting lexical analysis of user input:"
                     << "\n";
  Logger::instance() << input << "\n";

  std::vector<Token> tokens{};
  if (input.empty())
  {
    Logger::instance() << "Error : empty input for Lexer, can not process."
                       << "\n";
  }
  tokens.reserve(input.size() / 3);  // Assuming the most part of tokens are of 3 characters long.

  auto cursor = input.cbegin();
  while (cursor != input.cend())
  {
    auto substring_end = cursor + 1;
    while (*substring_end != SPACE && substring_end != input.cend())
    {
      ++substring_end;
    }
    std::string const substring(cursor, substring_end);
    tokens.emplace_back(parseSubstring(substring));
    cursor = substring_end;
    if (cursor != input.cend())
    {
      ++cursor;
    }
  }
  Logger::instance() << "Lexical analysis completed."
                     << "\n";
  return tokens;
}

/**
 * @brief Print token values until maximum print length reached
 * @param tokens
 */
void Lexer::printTokens(const std::vector<Token> &tokens)
{
  for (size_t i{0}; i < std::min(tokens.size(), MAX_LINE_WIDTH_FOR_PRINTING); ++i)
  {
    Logger::instance() << tokens[i].value << " ";
  }
  if (tokens.size() > MAX_LINE_WIDTH_FOR_PRINTING)
  {
    Logger::instance() << " ...";
  }
}

/**
 * @brief Converts a lexem substring to a parsed Token.
 * @param substring
 * @return
 */
Token Lexer::parseSubstring(const std::string &substring)
{
  if (FIXED_LEXEMS.find(substring) != FIXED_LEXEMS.cend())
  {
    return Token{FIXED_LEXEMS.at(substring), substring};
  }

  if (substring.find(FILENAME_SIGN) != std::string::npos &&
      substring.size() <= MAX_ALLOWED_FILENAME_LENGTH)
  {
    return Token{Lexem::FILENAME, substring};
  }

  Logger::instance() << "Error: unknown lexem '" << substring << "' found!"
                     << "\n";
  return Token{Lexem::UNKNOWN, substring};
}
