#include "ASTConsumerWrapper.h"

ASTConsumerWrapper::ASTConsumerWrapper(clang::ASTContext *context,
                                       clang::SourceManager &sourceManager,
                                       std::string *output)
    : _context(context), _sourceManager(sourceManager),
      vVisitor(context, &_VisitedContainers, &_memberLocs), _output(output) {
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

  GetOutput();
}

// edit this function if a new Matcher is created,
// currently placeholder until I have time to find a better solution
void ASTConsumerWrapper::constructMatchers() {
  _matchers.push_back(std::make_unique<MallocMatcher>());
  _matchers.push_back(std::make_unique<NewMatcher>());
  _matchers.push_back(std::make_unique<ReAllocMatcher>());
  _matchers.push_back(std::make_unique<FunctionAllocMatcher>());
  _matchers.push_back(std::make_unique<UninitializedMatcher>());
  auto containerMatcher = std::make_unique<ContainerMatcher>();
  containerMatcher.get()->setVisitedContainers(&_VisitedContainers);
  _matchers.push_back(std::move(containerMatcher));
  _matchers.push_back(std::make_unique<LinkedlistImplMatcher>());
  _matchers.push_back(std::make_unique<VectorImplMatcher>());
}

void ASTConsumerWrapper::GetOutput() {
  std::ostringstream os;
  for (size_t i = 0; i < _matchers.size(); i++) {
    os << _matchers.at(i).get()->writeOutput();
  }

  *_output = os.str();
}