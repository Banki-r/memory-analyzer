
#pragma once

#include<clang/AST/ASTConsumer.h>
#include<clang/Frontend/CompilerInstance.h>
#include<clang/ASTMatchers/ASTMatchFinder.h>

#include <cases/MallocMatcher.cpp>

class ASTConsumerWrapper : public clang::ASTConsumer
{
private:
    clang::ASTContext *_context;
    clang::ast_matchers::MatchFinder _matcher;
    clang::SourceManager &_sourceManager;
    MallocMatcher _mMatcher;
public:
    ASTConsumerWrapper(clang::ASTContext *context, clang::SourceManager &sourceManager);
    void HandleTranslationUnit(clang::ASTContext &context) override;
};