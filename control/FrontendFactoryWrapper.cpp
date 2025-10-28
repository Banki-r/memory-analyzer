
#include "FrontendFactoryWrapper.h"

FrontendFactoryWrapper::FrontendFactoryWrapper(std::string *output)
    : _output(output) {}

std::unique_ptr<clang::FrontendAction> FrontendFactoryWrapper::create() {
  return std::make_unique<FrontendActionWrapper>(_output);
}