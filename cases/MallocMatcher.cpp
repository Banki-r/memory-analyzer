#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"

using namespace clang;
using namespace clang::ast_matchers;

class MallocMatcher : public clang::ast_matchers::MatchFinder::MatchCallback
{
private:
    clang::ast_matchers::StatementMatcher _matcher = declStmt(hasDescendant(callExpr(callee(functionDecl(hasName("malloc")))))).bind("malloc");

public:
    
    virtual void run(const MatchFinder::MatchResult &result) override
    {
        auto malloc = result.Nodes.getNodeAs<DeclStmt>("malloc");
        llvm::outs() << "RES: " << malloc->getBeginLoc().printToString(result.Context->getSourceManager()) << "\n";
        if(malloc)
        {
            llvm::outs() << "got something! :)" << "\n";
        }
    }

    clang::ast_matchers::StatementMatcher getMatcher() { return _matcher;};
};