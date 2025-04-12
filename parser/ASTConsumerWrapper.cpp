
#include <clang/AST/RecursiveASTVisitor.h>
#include "ASTVisitorWrapper.h"
#include "ASTConsumerWrapper.h"

#include <iostream>
ASTConsumerWrapper::ASTConsumerWrapper(clang::ASTContext *context)
{
    _context = context;
}

void ASTConsumerWrapper::HandleTranslationUnit(clang::ASTContext &ctx) {
    clang::TranslationUnitDecl *tuDecl = ctx.getTranslationUnitDecl();
    ASTVisitorWrapper visitor;
    visitor.TraverseDecl(tuDecl);
}