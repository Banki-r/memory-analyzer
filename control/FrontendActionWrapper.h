#pragma once

#include <clang/Frontend/FrontendAction.h>
#include <clang/AST/ASTConsumer.h>

class FrontendActionWrapper : public clang::ASTFrontendAction
{
public:
    FrontendActionWrapper() = default;
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CompInst, llvm::StringRef InFile) override;
    void ExecuteAction() override {};
    bool usesPreprocessorOnly() const override {};
    bool BeginSourceFileAction(clang::CompilerInstance& CompInst) override;
    void EndSourceFileAction() override;

};