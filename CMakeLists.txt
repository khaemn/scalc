cmake_minimum_required(VERSION 3.5)

project(scalc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(HEADERS
  include/types.hpp
  include/node.hpp
  include/functions.hpp
  include/ops.hpp
  include/graph.hpp
  include/lexer.hpp
  include/logger.hpp

  test/predefined_data.hpp
  )

set(SOURCES
  main.cpp
  src/functions.cpp
  src/node.cpp
  src/graph.cpp
  src/lexer.cpp
  src/ops.cpp
  )

add_executable(scalc ${HEADERS} ${SOURCES})
