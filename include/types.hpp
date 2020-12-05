#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using DataType       = int64_t;
using MatchMap       = std::unordered_map<DataType, size_t>;
using Set            = std::unordered_set<DataType>;
using SetPtr         = std::shared_ptr<Set>;
using SetPtrEnsemble = std::vector<SetPtr>;

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
