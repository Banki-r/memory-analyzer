
#pragma once

#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>

#include <cases/matchers/FunctionAllocMatcher.cpp>
#include <cases/matchers/IMatcher.h>
#include <cases/matchers/MallocMatcher.cpp>
#include <cases/matchers/NewMatcher.cpp>
#include <cases/matchers/ReAllocMatcher.cpp>
#include <cases/matchers/UninitializedMatcher.cpp>
#include <cases/visitors/IASTVisitor.h>
#include <cases/visitors/VectorVisitor.cpp>

class ASTConsumerWrapper : public clang::ASTConsumer {
private:
  clang::ASTContext *_context;
  clang::ast_matchers::MatchFinder _matchFinder;
  clang::SourceManager &_sourceManager;
  std::vector<std::unique_ptr<IMatcher>> _matchers;

  VectorVisitor vVisitor;

  void constructMatchers();

public:
  ASTConsumerWrapper(clang::ASTContext *context,
                     clang::SourceManager &sourceManager);
  void HandleTranslationUnit(clang::ASTContext &context) override;
};