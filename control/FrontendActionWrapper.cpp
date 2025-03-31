#include "FrontendActionWrapper.h"
#include <parser/ASTConsumerWrapper.h>

std::unique_ptr<clang::ASTConsumer> FrontendActionWrapper::CreateASTConsumer(clang::CompilerInstance& CompInst, llvm::StringRef InFile)
{
    llvm::outs() << "Making ASTConsumer...\n";
    
    return std::make_unique<ASTConsumerWrapper>(&CompInst.getASTContext());
}

bool FrontendActionWrapper::BeginSourceFileAction(clang::CompilerInstance& CompInst)
{
    llvm::outs() << "Processing file...\n";
    return true;
}

void FrontendActionWrapper::EndSourceFileAction()
{
    ASTFrontendAction::EndSourceFileAction();
    llvm::outs() << "Finished processing file...\n";
}