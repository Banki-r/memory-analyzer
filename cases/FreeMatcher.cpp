#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include <vector>
#include <datatypes/allocedPointer.h>

using namespace clang;
using namespace clang::ast_matchers;

class FreeMatcher : public clang::ast_matchers::MatchFinder::MatchCallback
{
private:
    clang::ast_matchers::StatementMatcher _matcher = callExpr(hasDescendant(implicitCastExpr(hasDescendant(declRefExpr().bind("var")))),
    hasDescendant(callExpr(callee(functionDecl(hasName("free")))))).bind("free");
public:
    
    virtual void run(const MatchFinder::MatchResult &result) override
    {
        auto free = result.Nodes.getNodeAs<CallExpr>("free");
        auto var = result.Nodes.getNodeAs<DeclRefExpr>("var");

        if(free && var)
        {
            llvm::outs() << "Free found for variable: " << var->getNameInfo().getAsString() 
            << " at line: " << free->getBeginLoc().printToString(result.Context->getSourceManager()) << "\n";
        }else
        {
            llvm::outs() << "nothing \n";
        }
    }

    clang::ast_matchers::StatementMatcher getMatcher() { return _matcher;}

};