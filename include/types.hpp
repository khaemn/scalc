#pragma once

#include <cstdint>
#include <memory>
#include <vector>

using DataType   = int64_t;
using DataVector = std::vector<DataType>;
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
  FILENAME,
  //
  SPACE,
  UNKNOWN
};

struct Token
{
  Lexem       lexem;
  std::string value;
};
