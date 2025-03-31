#include "ASTConsumerWrapper.h"

void ASTConsumerWrapper::HandleTranslationUnit(clang::ASTContext& Context)
{
    llvm::outs() << "Handling translation unit...\n";
    clang::TranslationUnitDecl* transUnit = Context.getTranslationUnitDecl();
    visitor.TraverseDecl(transUnit);
}
