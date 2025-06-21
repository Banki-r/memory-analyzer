#include "IMatcher.h"
#include <vector>
#include <datatypes/AllocedPointer.h>

using namespace clang;
using namespace clang::ast_matchers;

class NewMatcher : public IMatcher
{
private:
    StatementMatcher _mMatcher = declStmt(hasDescendant(varDecl(hasDescendant(cxxNewExpr()))
    .bind("var"))).bind("new");
    StatementMatcher _fMatcher = cxxDeleteExpr(hasDescendant(implicitCastExpr(hasDescendant(declRefExpr()
    .bind("var"))))).bind("delete");

    std::vector<AllocedPointer> _allocedPointers;
    std::vector<StatementMatcher> _matchers = {_mMatcher, _fMatcher};
    std::list<int> toRemove;
    
    void removeFromVector()
    {
        for(int ind : toRemove)
        {
            _allocedPointers.erase(_allocedPointers.begin() + ind);
        }
    }
public:
    
    virtual void run(const MatchFinder::MatchResult &result) override
    {
        auto newNode = result.Nodes.getNodeAs<DeclStmt>("new");
        auto newVar = result.Nodes.getNodeAs<VarDecl>("var");

        auto deleteNode = result.Nodes.getNodeAs<CXXDeleteExpr>("delete");
        auto deleteVar = result.Nodes.getNodeAs<DeclRefExpr>("var");

        if(newNode && newVar)
        {
            AllocedPointer ap;
            ap.allocLine = newNode->getBeginLoc().printToString(result.Context->getSourceManager());
            if(ap.allocLine.find(".cpp") != std::string::npos)
            {
                ap.name = newVar->getNameAsString();
                ap.freeLine = "";
                _allocedPointers.push_back(ap);
            }
        }

        if(deleteNode)
        {
            std::string varName = deleteVar->getNameInfo().getAsString();
            std::string freeLine = deleteNode->getBeginLoc().printToString(result.Context->getSourceManager());
            
            for( size_t i = 0; i <_allocedPointers.size(); ++i)
            {
                if(_allocedPointers.at(i).name == varName)
                {
                    _allocedPointers.at(i).freeLine = freeLine;
                    toRemove.push_back(i);
                }
            }
        }

        removeFromVector();
    }

    virtual std::vector<StatementMatcher> getMatchers() override
    {
        return _matchers;
    }

    virtual void writeOutput() override 
    {
        for(AllocedPointer element : _allocedPointers)
        {
            llvm::outs() << "Variable " << element.name
            << " declared with new call at: " << element.allocLine 
            << " , is not deleted!\n";
        }
    }
};