#include "IMatcher.h"
#include "datatypes/ImplementedLinkedlist.h"
#include "datatypes/MemberLoc.h"
#include <vector>

using namespace clang;
using namespace clang::ast_matchers;

class VectorImplMatcher : public IMatcher {
private:
  StatementMatcher _dMatcher =
      memberExpr(hasAncestor(compoundStmt(
                                 hasAncestor(cxxDestructorDecl().bind("destr")))
                                 .bind("compn")))
          .bind("element");
  std::vector<ImplementedContainer> _ImplementedVectors;
  std::vector<StatementMatcher> _matchers = {_dMatcher};

  void removeFromVector() {
    _ImplementedVectors.erase(std::remove_if(_ImplementedVectors.begin(),
                                             _ImplementedVectors.end(),
                                             [](ImplementedContainer i) {
                                               return i.deletedProperly;
                                             }),
                              _ImplementedVectors.end());
  }

public:
  virtual void run(const MatchFinder::MatchResult &result) override {
    auto elementVar = result.Nodes.getNodeAs<MemberExpr>("element");
    auto destr = result.Nodes.getNodeAs<CXXDestructorDecl>("destr");
    auto compn = result.Nodes.getNodeAs<CompoundStmt>("compn");

    if (destr && compn) {
      if (result.SourceManager->isInMainFile(destr->getBeginLoc()) &&
          caseInsensitiveSubstrSearch(destr->getDeclName().getAsString(),
                                      "vector")) {
        if (compn->body_empty()) {
          ImplementedContainer iv;
          iv.sizeVarName = UNKOWN;
          iv.deletedProperly = false;
          iv.loc = compn->getBeginLoc().printToString(*result.SourceManager);
          _ImplementedVectors.push_back(iv);
        }
      }
    }

    if (elementVar && destr) {
      if (result.SourceManager->isInMainFile(elementVar->getBeginLoc()) &&
          caseInsensitiveSubstrSearch(destr->getDeclName().getAsString(),
                                      "vector")) {
        if (elementVar->isArrow()) {
          ImplementedContainer iv;
          iv.sizeVarName = elementVar->getMemberNameInfo().getAsString();
          iv.loc = elementVar->getMemberDecl()->getLocation().printToString(
              *result.SourceManager);
          auto parents = result.Context->getParents(*elementVar);
          auto parent = parents[0].get<clang::Stmt>();
          if (const CXXDeleteExpr *delexpr = dyn_cast<CXXDeleteExpr>(parent)) {
            iv.deletedProperly = true;
          } else {
            iv.deletedProperly = false;
          }
          _ImplementedVectors.push_back(iv);
        }
      }
    }
  }

  virtual std::vector<StatementMatcher> getMatchers() override {
    return _matchers;
  }

  virtual void writeOutput() override {
    removeFromVector();
    for (std::vector<ImplementedContainer>::iterator
             it = _ImplementedVectors.begin(),
             end = _ImplementedVectors.end();
         it != end; ++it) {

      if (it.base()->sizeVarName == UNKOWN) {
        llvm::outs() << "Vector destructor at " << it.base()->loc
                     << " propably doesn't delete everything.\n";
      } else {
        llvm::outs() << "Vector destructor propably doesn't delete everything."
                     << " Try using variable " << it.base()->loc
                     << " in the destructor.\n";
      }
    }
  }
};