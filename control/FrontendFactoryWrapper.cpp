
#include "FrontendFactoryWrapper.h"
#include "FrontendActionWrapper.h"

FrontendFactoryWrapper::FrontendFactoryWrapper() {

}

std::unique_ptr<clang::FrontendActionWrapper> FrontendFactoryWrapper::create() {
    return std::make_unique<FrontendActionWrapper>();
}