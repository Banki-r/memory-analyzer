#include "IMatcher.h"
#include "datatypes/MemberLoc.h"
#include "datatypes/ImplementedLinkedlist.h"
#include <vector>

using namespace clang;
using namespace clang::ast_matchers;

class LinkedlistImplMatcher : public IMatcher {
private:
  StatementMatcher _cMatcher = integerLiteral(hasParent(cxxConstructorDecl().bind("init")));
  StatementMatcher _dwhileMatcher = whileStmt(hasDescendant(memberExpr().bind("whileVar")), hasCondition(binaryOperator().bind("whileOp")), hasAncestor(compoundStmt(hasAncestor(cxxDestructorDecl())))).bind("whileStmt");
  StatementMatcher _dforMatcher = forStmt(hasDescendant(memberExpr().bind("forVar")), hasCondition(binaryOperator().bind("forOp")), hasAncestor(compoundStmt(hasAncestor(cxxDestructorDecl())))).bind("forStmt");
  std::vector<ImplementedLinkedlist> _ImplementedLinkedlists;
  std::vector<StatementMatcher> _matchers = {_cMatcher, _dwhileMatcher, _dforMatcher};

  void removeFromVector() {
    _ImplementedLinkedlists.erase(std::remove_if(_ImplementedLinkedlists.begin(),
                                             _ImplementedLinkedlists.end(),
                                             [](ImplementedLinkedlist i) {
                                               return i.deletedProperly;
                                             }),
                              _ImplementedLinkedlists.end());
  }

public:
  virtual void run(const MatchFinder::MatchResult &result) override {
    auto initCxx = result.Nodes.getNodeAs<CXXConstructorDecl>("init");

    auto deleteWhile = result.Nodes.getNodeAs<WhileStmt>("whileStmt");
    auto opWhile = result.Nodes.getNodeAs<BinaryOperator>("whileOp");
    auto whileVar = result.Nodes.getNodeAs<MemberExpr>("whileVar");
    
    auto deleteFor = result.Nodes.getNodeAs<ForStmt>("forStmt");
    auto opFor = result.Nodes.getNodeAs<BinaryOperator>("forOp");
    auto forVar = result.Nodes.getNodeAs<MemberExpr>("forVar");

    if(initCxx){
      for(clang::CXXCtorInitializer* init : initCxx->inits())
      {
        if(init->isMemberInitializer())
        {
          clang::Expr* initExpr = init->getInit();
          if(initExpr)
          {
            if(clang::IntegerLiteral* intLit = clang::dyn_cast<clang::IntegerLiteral>(initExpr))
            {
              ImplementedLinkedlist ill;
              ill.sizeVarName = init->getMember()->getNameAsString();
              ill.loc = init->getMember()->getLocation().printToString(*result.SourceManager);
              ill.deletedProperly = false;
              _ImplementedLinkedlists.push_back(ill);
            }
          }
        }
      }
    }

    if(deleteWhile && opWhile && whileVar)
    {
      if(opWhile->getOpcode() == clang::BinaryOperator::Opcode::BO_GT || opWhile->getOpcode() == clang::BinaryOperator::Opcode::BO_GE ||
         opWhile->getOpcode() == clang::BinaryOperator::Opcode::BO_LT || opWhile->getOpcode() == clang::BinaryOperator::Opcode::BO_LE)
      {
        for (auto iter = _ImplementedLinkedlists.begin(); iter != _ImplementedLinkedlists.end(); ++iter)
        {
          if(iter->sizeVarName == whileVar->getMemberNameInfo().getAsString()){
            iter->deletedProperly = true;
          }
        }
      }
    }else if(deleteFor && opFor && forVar)
    {
      if(opFor->getOpcode() == clang::BinaryOperator::Opcode::BO_GT || opFor->getOpcode() == clang::BinaryOperator::Opcode::BO_GE ||
         opFor->getOpcode() == clang::BinaryOperator::Opcode::BO_LT || opFor->getOpcode() == clang::BinaryOperator::Opcode::BO_LE)
      {
        for (auto iter = _ImplementedLinkedlists.begin(); iter != _ImplementedLinkedlists.end(); ++iter)
        {
          if(iter->sizeVarName == forVar->getMemberNameInfo().getAsString()){
            iter->deletedProperly = true;
          }
        }
      }
    }


  }

  virtual std::vector<StatementMatcher> getMatchers() override {
    return _matchers;
  }

  virtual void writeOutput() override {
    removeFromVector();
    for (std::vector<ImplementedLinkedlist>::iterator
             it = _ImplementedLinkedlists.begin(),
             end = _ImplementedLinkedlists.end();
         it != end; ++it) {
      llvm::outs() << "Linked List destructor propably doesn't delete everything."
                   << " Try using variable "
                   << it.base()->loc << " in the destructor.\n";
    }
  }
};