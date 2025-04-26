#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include <vector>
#include <datatypes/allocedPointer.h>

using namespace clang;
using namespace clang::ast_matchers;

class MallocMatcher : public clang::ast_matchers::MatchFinder::MatchCallback
{
private:
    clang::ast_matchers::StatementMatcher _matcher = declStmt(hasDescendant(varDecl().bind("var")),
    hasDescendant(callExpr(callee(functionDecl(hasName("malloc")))))).bind("malloc");
    std::vector<AllocedPointer> _allocedPointers;
public:
    
    virtual void run(const MatchFinder::MatchResult &result) override
    {
        auto malloc = result.Nodes.getNodeAs<DeclStmt>("malloc");
        auto var = result.Nodes.getNodeAs<VarDecl>("var");
        if(malloc && var)
        {
            AllocedPointer ap;
            ap.allocLine = malloc->getBeginLoc().printToString(result.Context->getSourceManager());
            ap.name = var->getNameAsString();
            ap.freeLine = "";
            _allocedPointers.push_back(ap);
            // For debugging:
            
            llvm::outs() << "Variable '" << ap.name << "' has been declared using a malloc call"
            << " at line " << ap.allocLine << "\n";
            
        }
    }

    clang::ast_matchers::StatementMatcher getMatcher() { return _matcher;}

    std::vector<AllocedPointer>* getPointers() { return &_allocedPointers;}
};