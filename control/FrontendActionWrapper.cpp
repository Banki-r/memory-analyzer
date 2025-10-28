#include "FrontendActionWrapper.h"

std::unique_ptr<clang::ASTConsumer>
FrontendActionWrapper::CreateASTConsumer(clang::CompilerInstance &ci,
                                         llvm::StringRef file) {
  return std::make_unique<ASTConsumerWrapper>(&ci.getASTContext(),
                                              ci.getSourceManager(), _output);
}

FrontendActionWrapper::FrontendActionWrapper(std::string *output)
    : _output(output) {}
