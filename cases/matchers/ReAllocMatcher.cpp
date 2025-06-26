#include "IMatcher.h"
#include <vector>
#include <datatypes/ReAllocedPointer.h>

using namespace clang;
using namespace clang::ast_matchers;

class ReAllocMatcher : public IMatcher
{
private:
    StatementMatcher _aMatcher = declStmt(hasDescendant(varDecl().bind("var"))).bind("line");
    StatementMatcher _rMatcher = binaryOperator(hasDescendant(declRefExpr().bind("var"))).bind("line");

    std::vector<ReAllocedPointer> _reAllocedPointers;
    std::vector<StatementMatcher> _matchers = {_aMatcher, _rMatcher};
    std::list<int> toRemove;

    void removeFromVector()
    {
        for(int i = 0; i < _reAllocedPointers.size(); ++i)
        {
            if(_reAllocedPointers[i].reAllocedLine.empty())
            {
                toRemove.push_back(i);
            }
        }
        for(int ind : toRemove)
        {
            _reAllocedPointers.erase(_reAllocedPointers.begin() + ind);
        }
    }
public:
    
    virtual void run(const MatchFinder::MatchResult &result) override
    {
        auto allocNode = result.Nodes.getNodeAs<DeclStmt>("line");
        auto allocVar = result.Nodes.getNodeAs<VarDecl>("var");

        auto reallocNode = result.Nodes.getNodeAs<BinaryOperator>("line");
        auto reallocVar = result.Nodes.getNodeAs<DeclRefExpr>("var");

        if(allocNode && allocVar)
        {
            ReAllocedPointer ap;
            ap.allocLine = allocNode->getBeginLoc().printToString(result.Context->getSourceManager());
            QualType type = allocVar->getType();
            std::string typeString = type.getAsString();
            if(ap.allocLine.find(".cpp") != std::string::npos &&
                typeString.find("*") != std::string::npos)
            {
                ap.name = allocVar->getNameAsString();
                ap.reAllocedLine = "";
                _reAllocedPointers.push_back(ap);
            }
        }
        if(reallocNode && reallocNode->isAssignmentOp())
        {
            std::string varName = reallocVar->getNameInfo().getAsString();
            std::string reallocLine = reallocNode->getBeginLoc().printToString(result.Context->getSourceManager());
            
            for( size_t i = 0; i <_reAllocedPointers.size(); ++i)
            {
                if(_reAllocedPointers.at(i).name == varName)
                {
                    _reAllocedPointers.at(i).reAllocedLine = reallocLine;
                }
            }
        }
    }

    virtual std::vector<StatementMatcher> getMatchers() override
    {
        return _matchers;
    }

    virtual void writeOutput() override 
    {
        removeFromVector();
        for(ReAllocedPointer element : _reAllocedPointers)
        {
            llvm::outs() << "Pointer " << element.name
            << " declared at: " << element.allocLine 
            << " , is hidden by a reallocation at: " << element.reAllocedLine << " !\n";
        }
    }
};