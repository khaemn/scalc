cmake_minimum_required(VERSION 3.5)

project(scalc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(HEADERS
  include/types.hpp
  include/node.hpp
  include/engine.hpp
  include/ops.hpp
  include/expression.hpp
  include/lexer.hpp
  include/logger.hpp
  )

set(SOURCES
  src/main.cpp
  src/engine.cpp
  src/node.cpp
  src/expression.cpp
  src/lexer.cpp
  src/ops.cpp
  )

add_executable(scalc ${HEADERS} ${SOURCES})
