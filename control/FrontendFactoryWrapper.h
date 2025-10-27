#pragma once

#include <clang/Tooling/Tooling.h>
#include "control/FrontendActionWrapper.h"

class FrontendFactoryWrapper : public clang::tooling::FrontendActionFactory {
public:
  FrontendFactoryWrapper();
  std::unique_ptr<clang::FrontendAction> create() override;
};