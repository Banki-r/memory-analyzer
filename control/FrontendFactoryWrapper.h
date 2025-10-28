#pragma once

#include "control/FrontendActionWrapper.h"
#include <clang/Tooling/Tooling.h>

class FrontendFactoryWrapper : public clang::tooling::FrontendActionFactory {
private:
  std::string *_output;

public:
  FrontendFactoryWrapper(std::string *output);
  std::unique_ptr<clang::FrontendAction> create() override;
};