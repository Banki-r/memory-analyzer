#pragma once

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"

using namespace clang;
using namespace clang::ast_matchers;

class IMatcher : public MatchFinder::MatchCallback
{
public:
    virtual void run(const MatchFinder::MatchResult &result) override = 0;
    virtual std::vector<StatementMatcher> getMatchers() = 0;
    virtual void writeOutput() = 0;
};
