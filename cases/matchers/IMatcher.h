#pragma once

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"

using namespace clang;
using namespace clang::ast_matchers;

class IMatcher : public MatchFinder::MatchCallback
{
protected:
    const FunctionDecl* _allocFunc;
    const FunctionDecl* _reallocFunc;

    template<typename T>
    const FunctionDecl* getParentFunction(const MatchFinder::MatchResult &result, T &Node)
    {
        clang::DynTypedNodeList parentNodeList = result.Context->getParents(Node);
        clang::DynTypedNode parentNode;
        while(!parentNodeList.empty())
        {
            parentNode = parentNodeList[0];
            if(const FunctionDecl* Parent = parentNode.get<FunctionDecl>())
            {
                return Parent;
            }
            parentNodeList = result.Context->getParents(parentNode);
            
        }
        return nullptr;
    }

public:
    virtual void run(const MatchFinder::MatchResult &result) override = 0;
    virtual std::vector<StatementMatcher> getMatchers() = 0;
    virtual void writeOutput() = 0;
};
