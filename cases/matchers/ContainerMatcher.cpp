#include "IMatcher.h"
#include "datatypes/MemberLoc.h"
#include "datatypes/VisitedContainer.h"
#include <vector>

using namespace clang;
using namespace clang::ast_matchers;

class ContainerMatcher : public IMatcher {
private:
  StatementMatcher _cMatcher = declStmt(
      hasDescendant(varDecl(hasDescendant(cxxConstructExpr(hasDescendant(
                                initListExpr().bind("constrList")))))
                        .bind("allocNode")));
  StatementMatcher _dMatcher =
      cxxDeleteExpr(hasDescendant(implicitCastExpr()),
                    hasDescendant(declRefExpr().bind("deleted")));
  std::vector<VisitedContainer> *_VisitedContainers;
  std::vector<StatementMatcher> _matchers = {_cMatcher, _dMatcher};

  void removeFromVector() {
    _VisitedContainers->erase(std::remove_if(_VisitedContainers->begin(),
                                             _VisitedContainers->end(),
                                             [](VisitedContainer i) {
                                               return i.frees == i.push_backs;
                                             }),
                              _VisitedContainers->end());
  }

public:
  virtual void run(const MatchFinder::MatchResult &result) override {

    auto allocNode = result.Nodes.getNodeAs<VarDecl>("allocNode");
    auto constrList = result.Nodes.getNodeAs<InitListExpr>("constrList");

    auto deletedNode = result.Nodes.getNodeAs<DeclRefExpr>("deleted");

    if (allocNode && constrList) {
      for (std::vector<VisitedContainer>::iterator
               it = _VisitedContainers->begin(),
               end = _VisitedContainers->end();
           it != end; ++it) {
        if (it.base()->objName == allocNode->getNameAsString()) {
          it.base()->push_backs += constrList->getNumInits();
        }
      }
    }

    if (deletedNode) {
      for (std::vector<VisitedContainer>::iterator
               it = _VisitedContainers->begin(),
               end = _VisitedContainers->end();
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
    for (std::vector<VisitedContainer>::iterator
             it = _VisitedContainers->begin(),
             end = _VisitedContainers->end();
         it != end; ++it) {
      llvm::outs() << it.base()->objName
                   << " container isn't deleted properly at "
                   << it.base()->allocedLine
                   << ". deletions: " << it.base()->frees
                   << ", inserts: " << it.base()->push_backs << ". \n";
    }
  }

  void setVisitedContainers(std::vector<VisitedContainer> *VisitedContainers) {
    _VisitedContainers = VisitedContainers;
  }
};