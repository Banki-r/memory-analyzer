#include "IMatcher.h"
#include <vector>
#include <datatypes/AllocedPointer.h>

using namespace clang;
using namespace clang::ast_matchers;

class MallocMatcher : public IMatcher
{
private:
    StatementMatcher _mMatcher = declStmt(hasDescendant(varDecl().bind("var")),
    hasDescendant(callExpr(callee(functionDecl(hasName("malloc")))))).bind("malloc");
    StatementMatcher _fMatcher = callExpr(callee(functionDecl(matchesName("free"))), 
    hasDescendant(implicitCastExpr(hasDescendant(implicitCastExpr(hasDescendant(declRefExpr().bind("var"))))))).bind("free");
    StatementMatcher _retMatcher = returnStmt(hasDescendant(declRefExpr().bind("retVar"))).bind("return");
    
    std::vector<AllocedPointer> _allocedPointers;
    std::vector<StatementMatcher> _matchers = {_retMatcher, _mMatcher, _fMatcher};
    
    void removeFromVector()
    {

        _allocedPointers.erase(std::remove_if(_allocedPointers.begin(), 
        _allocedPointers.end(), [](AllocedPointer i) { return !i.freeLine.empty();}),
        _allocedPointers.end());
    }

public:
    
    virtual void run(const MatchFinder::MatchResult &result) override
    {
        auto malloc = result.Nodes.getNodeAs<DeclStmt>("malloc");
        auto mallocVar = result.Nodes.getNodeAs<VarDecl>("var");

        auto free = result.Nodes.getNodeAs<CallExpr>("free");
        auto freeVar = result.Nodes.getNodeAs<DeclRefExpr>("var");

        auto retVar = result.Nodes.getNodeAs<DeclRefExpr>("retVar");

        if(malloc && mallocVar)
        {
            _allocFunc = getParentFunction(result, *malloc);
            AllocedPointer ap;
            ap.allocLine = malloc->getBeginLoc().printToString(result.Context->getSourceManager());
            ap.name = mallocVar->getNameAsString();
            ap.freeLine = "";
            _allocedPointers.push_back(ap);
            // For debugging:
            
            /*llvm::outs() << "Variable '" << ap.name << "' has been declared using a malloc call"
            << " at line " << ap.allocLine << "\n";*/
            
        }

        if(retVar)
        {
            auto retVarName = retVar->getNameInfo().getAsString();
            for( size_t i = 0; i <_allocedPointers.size(); ++i)
            {
                if(_allocedPointers.at(i).name == retVarName)
                {
                    _allocedPointers.at(i).freeLine = "returned by function";
                }
            }
        }

        if(free)
        {
            _reallocFunc = getParentFunction(result, *free);
            if(_allocFunc == _reallocFunc)
            {
                std::string varName = freeVar->getNameInfo().getAsString();
                std::string freeLine = free->getBeginLoc().printToString(result.Context->getSourceManager());
                // For debugging:
                /*llvm::outs() << "Free found for variable: " << varName
                << " at line: " << freeLine << "\n";*/
                
                for( size_t i = 0; i <_allocedPointers.size(); ++i)
                {
                    if(_allocedPointers.at(i).name == varName)
                    {
                        _allocedPointers.at(i).freeLine = freeLine;
                    }
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
        for(AllocedPointer element : _allocedPointers)
        {
            llvm::outs() << "Variable " << element.name
            << " declared with a malloc call at: " << element.allocLine 
            << " , is not freed up!\n";
        }
    }
};