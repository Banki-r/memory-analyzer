
#include "FrontendFactoryWrapper.h"
#include "FrontendActionWrapper.h"

FrontendFactoryWrapper::FrontendFactoryWrapper()
{

}

std::unique_ptr<clang::FrontendAction> FrontendFactoryWrapper::create()
{
    return std::make_unique<FrontendActionWrapper>();
}