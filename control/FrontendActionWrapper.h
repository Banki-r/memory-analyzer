#pragma once

#include <clang/Frontend/FrontendAction.h>
#include "parser/ASTConsumerWrapper.h"

class FrontendActionWrapper : public clang::ASTFrontendAction {
protected:
  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &ci, llvm::StringRef file) override;
};
