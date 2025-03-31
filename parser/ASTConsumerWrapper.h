#pragma once

#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>
#include "ASTVisitorWrapper.h"

class ASTConsumerWrapper : public clang::ASTConsumer
{
private:
    ASTVisitorWrapper visitor;
    clang::CompilerInstance* CompIn;
    llvm::StringRef InFile;

public:
    explicit ASTConsumerWrapper(clang::ASTContext* Context) : visitor(Context){};
    void HandleTranslationUnit(clang::ASTContext& Context) override;

};