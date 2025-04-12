
#pragma once

#include<clang/AST/ASTConsumer.h>
#include<clang/Frontend/CompilerInstance.h>

class ASTConsumerWrapper : public clang::ASTConsumer {
private:
    clang::ASTContext *_context;
public:
    ASTConsumerWrapper(clang::ASTContext *context);
    void HandleTranslationUnit(clang::ASTContext &context) override;
};