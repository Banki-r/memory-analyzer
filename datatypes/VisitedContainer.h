#pragma once
#include <iostream>

struct VisitedContainer {
  std::string objName;
  std::string allocedLine;
  int push_backs = 0;
  int frees = 0;
};