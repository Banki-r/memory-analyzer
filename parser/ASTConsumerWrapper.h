
#pragma once

#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>

#include <cases/matchers/ContainerMatcher.cpp>
#include <cases/matchers/FunctionAllocMatcher.cpp>
#include <cases/matchers/IMatcher.h>
#include <cases/matchers/MallocMatcher.cpp>
#include <cases/matchers/NewMatcher.cpp>
#include <cases/matchers/ReAllocMatcher.cpp>
#include <cases/matchers/UninitializedMatcher.cpp>
#include <cases/visitors/ContainerVisitor.cpp>
#include <cases/visitors/IASTVisitor.h>

class ASTConsumerWrapper : public clang::ASTConsumer {
private:
  clang::ASTContext *_context;
  clang::ast_matchers::MatchFinder _matchFinder;
  clang::SourceManager &_sourceManager;
  std::vector<std::unique_ptr<IMatcher>> _matchers;

  ContainerVisitor vVisitor;
  std::vector<VisitedContainer> _VisitedContainers;
  std::vector<MemberLoc> _memberLocs;

  void constructMatchers();

public:
  ASTConsumerWrapper(clang::ASTContext *context,
                     clang::SourceManager &sourceManager);
  void HandleTranslationUnit(clang::ASTContext &context) override;
};