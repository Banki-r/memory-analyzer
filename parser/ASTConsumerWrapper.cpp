
#include <clang/AST/RecursiveASTVisitor.h>
#include "ASTConsumerWrapper.h"

#include <iostream>

void ASTConsumerWrapper::HandleTranslationUnit(clang::ASTContext &ctx) {
    clang::TranslationUnitDecl *tuDecl = ctx.getTranslationUnitDecl();
    ASTVisitorWrapper visitor;
    visitor.TraverseDecl(tuDecl);
}