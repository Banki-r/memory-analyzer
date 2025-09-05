#pragma once
#include <iostream>

struct VisitedVector {
  std::string objName;
  std::string allocedLine;
  int push_backs;
  int frees;
  bool deletedCorrectly;
};