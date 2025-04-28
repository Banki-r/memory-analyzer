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
    clang::ast_matchers::StatementMatcher _mMatcher = declStmt(hasDescendant(varDecl().bind("var")),
    hasDescendant(callExpr(callee(functionDecl(hasName("malloc")))))).bind("malloc");

    clang::ast_matchers::StatementMatcher _fMatcher = callExpr(callee(functionDecl(matchesName("free"))), 
    hasDescendant(implicitCastExpr(hasDescendant(implicitCastExpr(hasDescendant(declRefExpr().bind("var"))))))).bind("free");

    std::vector<AllocedPointer> _allocedPointers;
public:
    
    virtual void run(const MatchFinder::MatchResult &result) override
    {
        auto malloc = result.Nodes.getNodeAs<DeclStmt>("malloc");
        auto malloc_var = result.Nodes.getNodeAs<VarDecl>("var");

        auto free = result.Nodes.getNodeAs<CallExpr>("free");
        auto free_var = result.Nodes.getNodeAs<DeclRefExpr>("var");

        if(malloc && malloc_var)
        {
            AllocedPointer ap;
            ap.allocLine = malloc->getBeginLoc().printToString(result.Context->getSourceManager());
            ap.name = malloc_var->getNameAsString();
            ap.freeLine = "";
            _allocedPointers.push_back(ap);
            // For debugging:
            
            /*llvm::outs() << "Variable '" << ap.name << "' has been declared using a malloc call"
            << " at line " << ap.allocLine << "\n";*/
            
        }

        if(free)
        {
            std::string varName = free_var->getNameInfo().getAsString();
            std::string freeLine = free->getBeginLoc().printToString(result.Context->getSourceManager());
            // For debugging:
            /*llvm::outs() << "Free found for variable: " << varName
            << " at line: " << freeLine << "\n";*/
            
            for( int i = 0; i <_allocedPointers.size(); ++i)
            {
                if(_allocedPointers.at(i).name == varName)
                {
                    _allocedPointers.at(i).freeLine = freeLine;
                }
            }
        }
    }

    clang::ast_matchers::StatementMatcher getMallocMatcher() { return _mMatcher;}

    clang::ast_matchers::StatementMatcher getFreeMatcher() { return _fMatcher;}

    std::vector<AllocedPointer>* getPointers() { return &_allocedPointers;}
};