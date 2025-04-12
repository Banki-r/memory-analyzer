
#pragma once

#include<clang/AST/ASTConsumer.h>
#include<clang/Frontend/CompilerInstance.h>

class ASTConsumerWrapper : public clang::ASTConsumer {

public:
    ASTConsumerWrapper(clang::CompilerInstance &ci, llvm::StringRef file) {}
    void HandleTranslationUnit(clang::ASTContext &context) override;
};