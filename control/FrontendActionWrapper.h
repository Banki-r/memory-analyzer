#pragma once

#include "parser/ASTConsumerWrapper.h"
#include <clang/Frontend/FrontendAction.h>

class FrontendActionWrapper : public clang::ASTFrontendAction {
private:
  std::string *_output;

protected:
  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &ci, llvm::StringRef file) override;

public:
  FrontendActionWrapper(std::string *output);
};
