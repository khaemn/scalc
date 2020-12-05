#pragma once

#include <vector>

#include "engine.hpp"
#include "expression.hpp"
#include "types.hpp"

// [ SUM [ DIF a.txt b.txt c.txt ] [ INT b.txt c.txt ] ]
static const std::vector<Token> example{
    {Lexem::OPEN, "["},         {Lexem::SUM, "SUM"},        {Lexem::OPEN, "["},
    {Lexem::DIFF, "DIFF"},      {Lexem::FILENAME, "a.txt"}, {Lexem::FILENAME, "b.txt"},
    {Lexem::FILENAME, "c.txt"}, {Lexem::CLOSE, "]"},        {Lexem::OPEN, "["},
    {Lexem::INT, "INT"},        {Lexem::FILENAME, "b.txt"}, {Lexem::FILENAME, "c.txt"},
    {Lexem::CLOSE, "]"},        {Lexem::CLOSE, "]"}};

// [ SUM [ SUM [ SUM a.txt b.txt ] [ SUM b.txt c.txt ] ] [ SUM [ SUM c.txt d.txt ] [ SUM d.txt e.txt
// ] ] ]
static const std::vector<Token> example2{
    {Lexem::OPEN, "["},         {Lexem::SUM, "SUM"},        {Lexem::OPEN, "["},
    {Lexem::SUM, "SUM"},        {Lexem::OPEN, "["},         {Lexem::SUM, "SUM"},
    {Lexem::FILENAME, "a.txt"}, {Lexem::FILENAME, "b.txt"}, {Lexem::CLOSE, "]"},
    {Lexem::OPEN, "["},         {Lexem::SUM, "SUM"},        {Lexem::FILENAME, "b.txt"},
    {Lexem::FILENAME, "c.txt"}, {Lexem::CLOSE, "]"},        {Lexem::CLOSE, "]"},
    {Lexem::OPEN, "["},         {Lexem::SUM, "SUM"},        {Lexem::OPEN, "["},
    {Lexem::SUM, "SUM"},        {Lexem::FILENAME, "c.txt"}, {Lexem::FILENAME, "d.txt"},
    {Lexem::CLOSE, "]"},        {Lexem::OPEN, "["},         {Lexem::SUM, "SUM"},
    {Lexem::FILENAME, "d.txt"}, {Lexem::FILENAME, "e.txt"}, {Lexem::CLOSE, "]"},
    {Lexem::CLOSE, "]"},        {Lexem::CLOSE, "]"},
};

// [ SUM [ SUM [ SUM a.txt b.txt ] [ SUM c.txt e.txt ] ] [ SUM [ SUM d.txt ] ] ]
static const std::vector<Token> example3{
    {Lexem::OPEN, "["},         {Lexem::SUM, "SUM"},        {Lexem::OPEN, "["},
    {Lexem::SUM, "SUM"},        {Lexem::OPEN, "["},         {Lexem::SUM, "SUM"},
    {Lexem::FILENAME, "a.txt"}, {Lexem::FILENAME, "b.txt"}, {Lexem::CLOSE, "]"},
    {Lexem::OPEN, "["},         {Lexem::SUM, "SUM"},        {Lexem::FILENAME, "c.txt"},
    {Lexem::FILENAME, "e.txt"}, {Lexem::CLOSE, "]"},        {Lexem::CLOSE, "]"},
    {Lexem::OPEN, "["},         {Lexem::SUM, "SUM"},        {Lexem::OPEN, "["},
    {Lexem::SUM, "SUM"},        {Lexem::FILENAME, "d.txt"}, {Lexem::CLOSE, "]"},
    {Lexem::CLOSE, "]"},        {Lexem::CLOSE, "]"},
};

// [ SUM [ SUM [ SUM [ SUM [ SUM [ SUM [ SUM a.txt ] ] ] ] ] ] ]
static const std::vector<Token> example4{
    {Lexem::OPEN, "["},  {Lexem::SUM, "SUM"}, {Lexem::OPEN, "["},         {Lexem::SUM, "SUM"},
    {Lexem::OPEN, "["},  {Lexem::SUM, "SUM"}, {Lexem::OPEN, "["},         {Lexem::SUM, "SUM"},
    {Lexem::OPEN, "["},  {Lexem::SUM, "SUM"}, {Lexem::OPEN, "["},         {Lexem::SUM, "SUM"},
    {Lexem::OPEN, "["},  {Lexem::SUM, "SUM"}, {Lexem::FILENAME, "a.txt"}, {Lexem::CLOSE, "]"},
    {Lexem::CLOSE, "]"}, {Lexem::CLOSE, "]"}, {Lexem::CLOSE, "]"},        {Lexem::CLOSE, "]"},
    {Lexem::CLOSE, "]"}, {Lexem::CLOSE, "]"},
};

inline void testGraphBuildingFromData()
{
  Expression g;
  g.addNode<OperationType::INTEGER>("A", {}, Set{1, 2, 3});
  g.addNode<OperationType::INTEGER>("B", {}, Set{2, 3, 4});
  g.addNode<OperationType::INTEGER>("C", {}, Set{3, 4, 5});

  g.addNode<OperationType::FILEREADER>("A", {}, "a.txt");
  g.addNode<OperationType::FILEREADER>("B", {}, "b.txt");
  g.addNode<OperationType::FILEREADER>("C", {}, "c.txt");

  g.addNode<OperationType::INTERSECTION>("INT", {"B", "C"});
  g.addNode<OperationType::DIFFERENCE>("DIF", {"A", "B", "C"});
  g.addNode<OperationType::UNION>("SUM", {"DIF", "INT"});

  g.compile();
  printVector(g.evaluate("SUM"));
}
