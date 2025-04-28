
#include <clang/AST/RecursiveASTVisitor.h>
#include "ASTVisitorWrapper.h"
#include "ASTConsumerWrapper.h"

#include <iostream>
ASTConsumerWrapper::ASTConsumerWrapper(clang::ASTContext *context, clang::SourceManager &sourceManager)
: _context(context), _sourceManager(sourceManager)
{
    _matcher.addMatcher(_mMatcher.getMallocMatcher(), &_mMatcher);
    _matcher.addMatcher(_mMatcher.getFreeMatcher(), &_mMatcher);
}

void ASTConsumerWrapper::HandleTranslationUnit(clang::ASTContext &ctx) {
    _matcher.matchAST(*_context);
    clang::TranslationUnitDecl *tuDecl = ctx.getTranslationUnitDecl();
    ASTVisitorWrapper visitor(_context);
    visitor.TraverseDecl(tuDecl);
}