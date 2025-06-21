#include "ASTConsumerWrapper.h"

#include <iostream>

ASTConsumerWrapper::ASTConsumerWrapper(clang::ASTContext *context, clang::SourceManager &sourceManager)
: _context(context), _sourceManager(sourceManager)
{
    constructMatchers();
    for(int i = 0; i < _matchers.size(); ++i)
    {
        std::vector<clang::ast_matchers::StatementMatcher> matchers = _matchers.at(i).get()->getMatchers();
        for(int j = 0; j < matchers.size(); ++j)
        {
            _matchFinder.addMatcher(matchers.at(j), _matchers.at(i).get());
        }
    }
}

void ASTConsumerWrapper::HandleTranslationUnit(clang::ASTContext &ctx) {
    _matchFinder.matchAST(*_context);
    clang::TranslationUnitDecl *tuDecl = ctx.getTranslationUnitDecl();
    for(auto& matcher : _matchers)
    {
        matcher->writeOutput();
    }
}

// edit this function if a new Matcher is created,
// currently placeholder until I have time to find a better solution
void ASTConsumerWrapper::constructMatchers()
{
    _matchers.push_back(std::make_unique<MallocMatcher>());
    _matchers.push_back(std::make_unique<NewMatcher>());
}