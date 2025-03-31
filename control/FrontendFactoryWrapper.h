#pragma once

#include <clang/Tooling/Tooling.h>

class FrontendFactoryWrapper : public clang::tooling::FrontendActionFactory
{
public:
    std::unique_ptr<clang::FrontendAction> create() override;
};