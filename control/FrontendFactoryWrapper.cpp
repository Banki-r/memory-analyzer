#include "FrontendFactoryWrapper.h"
#include "FrontendActionWrapper.h"
#include <iostream>

std::unique_ptr<clang::FrontendAction> FrontendFactoryWrapper::create()
{
    llvm::outs() <<  "FrontendAction created...\n";
    return std::make_unique<FrontendActionWrapper>();
}