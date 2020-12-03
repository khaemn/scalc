#pragma once

#include <cstdint>
#include <memory>
#include <deque>
#include <vector>
#include <map>
#include <unordered_set>

using DataType   = int64_t;
using MatchMap   = std::map<DataType, size_t>;
using DataVector = std::unordered_set<DataType>;
using VectorPtr  = std::shared_ptr<DataVector>;
using InputData  = std::vector<VectorPtr>;

enum class Lexem
{
  OPEN,
  CLOSE,
  //
  SUM,
  INT,
  DIFF,
  //
  EQ,
  LE,
  GR,
  //
  FILENAME,
  //
  SPACE,
  INTEGER,
  UNKNOWN
};

struct Token
{
  Lexem       lexem;
  std::string value;
};
