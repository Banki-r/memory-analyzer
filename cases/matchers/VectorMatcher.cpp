#include "IMatcher.h"
#include "datatypes/MemberLoc.h"
#include "datatypes/VisitedVector.h"
#include <vector>

using namespace clang;
using namespace clang::ast_matchers;

class VectorMatcher : public IMatcher {
private:
  StatementMatcher _cMatcher = declStmt(
      hasSingleDecl(varDecl(hasDescendant(cxxConstructExpr(forEachDescendant(
                                declRefExpr().bind("constrVar")))))
                        .bind("allocNode")));
  StatementMatcher _dMatcher =
      cxxDeleteExpr(hasDescendant(implicitCastExpr()),
                    hasDescendant(declRefExpr().bind("deleted")));
  int foundInConstr = 0;
  std::vector<VisitedVector> *_visitedVectors;
  std::vector<StatementMatcher> _matchers = {_cMatcher, _dMatcher};

  void removeFromVector() {
    _visitedVectors->erase(
        std::remove_if(_visitedVectors->begin(), _visitedVectors->end(),
                       [](VisitedVector i) { return i.frees == i.push_backs; }),
        _visitedVectors->end());
  }

public:
  virtual void run(const MatchFinder::MatchResult &result) override {

    auto allocNode = result.Nodes.getNodeAs<VarDecl>("allocNode");
    auto constrVar = result.Nodes.getNodeAs<DeclRefExpr>("constrVar");

    auto deletedNode = result.Nodes.getNodeAs<DeclRefExpr>("deleted");

    if (allocNode && constrVar) {
      for (std::vector<VisitedVector>::iterator it = _visitedVectors->begin(),
                                                end = _visitedVectors->end();
           it != end; ++it) {
        if (it.base()->objName == allocNode->getNameAsString()) {
          ++foundInConstr;
          if (foundInConstr % 2 == 0) {
            it.base()->push_backs += 1;
          }
        }
      }
    }

    if (deletedNode) {
      for (std::vector<VisitedVector>::iterator it = _visitedVectors->begin(),
                                                end = _visitedVectors->end();
           it != end; ++it) {
        if (it.base()->objName == deletedNode->getNameInfo().getAsString()) {
          it.base()->frees += 1;
        }
      }
    }
  }

  virtual std::vector<StatementMatcher> getMatchers() override {
    return _matchers;
  }

  virtual void writeOutput() override {
    removeFromVector();
    for (std::vector<VisitedVector>::iterator it = _visitedVectors->begin(),
                                              end = _visitedVectors->end();
         it != end; ++it) {
      llvm::outs() << it.base()->objName << " vector isn't deleted properly at "
                   << it.base()->allocedLine << ".\n";
    }
  }

  void setVisitedVectors(std::vector<VisitedVector> *visitedVectors) {
    _visitedVectors = visitedVectors;
  }
};