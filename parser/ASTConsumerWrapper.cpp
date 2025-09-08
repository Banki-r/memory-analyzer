#include "ASTConsumerWrapper.h"

#include <iostream>

ASTConsumerWrapper::ASTConsumerWrapper(clang::ASTContext *context,
                                       clang::SourceManager &sourceManager)
    : _context(context), _sourceManager(sourceManager),
      vVisitor(context, &_visitedVectors, &_memberLocs) {
  constructMatchers();
  for (size_t i = 0; i < _matchers.size(); ++i) {
    std::vector<clang::ast_matchers::StatementMatcher> matchers =
        _matchers.at(i).get()->getMatchers();
    for (size_t j = 0; j < matchers.size(); ++j) {
      _matchFinder.addMatcher(matchers.at(j), _matchers.at(i).get());
    }
  }
}

void ASTConsumerWrapper::HandleTranslationUnit(clang::ASTContext &ctx) {
  clang::TranslationUnitDecl *tuDecl = ctx.getTranslationUnitDecl();
  vVisitor.TraverseDecl(tuDecl);

  _matchFinder.matchAST(*_context);
  for (auto &matcher : _matchers) {
    matcher->writeOutput();
  }
}

// edit this function if a new Matcher is created,
// currently placeholder until I have time to find a better solution
void ASTConsumerWrapper::constructMatchers() {
  _matchers.push_back(std::make_unique<MallocMatcher>());
  _matchers.push_back(std::make_unique<NewMatcher>());
  _matchers.push_back(std::make_unique<ReAllocMatcher>());
  _matchers.push_back(std::make_unique<FunctionAllocMatcher>());
  _matchers.push_back(std::make_unique<UninitializedMatcher>());
  auto vectorMatcher = std::make_unique<VectorMatcher>();
  vectorMatcher.get()->setVisitedVectors(&_visitedVectors);
  _matchers.push_back(std::move(vectorMatcher));
}