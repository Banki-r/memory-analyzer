
#include "FrontendFactoryWrapper.h"

FrontendFactoryWrapper::FrontendFactoryWrapper() {}

std::unique_ptr<clang::FrontendAction> FrontendFactoryWrapper::create() {
  return std::make_unique<FrontendActionWrapper>();
}