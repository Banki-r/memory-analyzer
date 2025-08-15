#include "IMatcher.h"
#include <datatypes/ReAllocedPointer.h>
#include <vector>

using namespace clang;
using namespace clang::ast_matchers;

class ReAllocMatcher : public IMatcher {
private:
  StatementMatcher _aMatcher =
      declStmt(hasDescendant(varDecl().bind("var"))).bind("line");
  StatementMatcher _rMatcher =
      binaryOperator(hasDescendant(declRefExpr().bind("var"))).bind("line");

  std::vector<ReAllocedPointer> _reAllocedPointers;
  std::vector<StatementMatcher> _matchers = {_aMatcher, _rMatcher};

  void removeFromVector() {

    _reAllocedPointers.erase(std::remove_if(_reAllocedPointers.begin(),
                                            _reAllocedPointers.end(),
                                            [](ReAllocedPointer i) {
                                              return i.reAllocedLine.empty();
                                            }),
                             _reAllocedPointers.end());
  }

public:
  virtual void run(const MatchFinder::MatchResult &result) override {
    auto allocNode = result.Nodes.getNodeAs<DeclStmt>("line");
    auto allocVar = result.Nodes.getNodeAs<VarDecl>("var");

    auto reallocNode = result.Nodes.getNodeAs<BinaryOperator>("line");
    auto reallocVar = result.Nodes.getNodeAs<DeclRefExpr>("var");

    if (allocNode && allocVar) {
      if (result.Context->getSourceManager().isWrittenInMainFile(
              allocNode->getBeginLoc()) &&
          allocVar->getType().getTypePtr()->isPointerType()) {
        ReAllocedPointer ap;
        ap.allocFunc = getParentFunction(result, *allocNode)->getNameAsString();
        ap.allocLine = allocNode->getBeginLoc().printToString(
            result.Context->getSourceManager());
        ap.name = allocVar->getNameAsString();
        ap.reAllocedLine = "";
        _reAllocedPointers.push_back(ap);
      }
    }

    if (reallocNode && reallocNode->isAssignmentOp()) {
      std::string reallocFunc =
          getParentFunction(result, *reallocNode)->getNameAsString();
      std::string varName = reallocVar->getNameInfo().getAsString();
      std::string reallocLine = reallocNode->getBeginLoc().printToString(
          result.Context->getSourceManager());
      for (size_t i = 0; i < _reAllocedPointers.size(); ++i) {
        if (_reAllocedPointers.at(i).name == varName &&
            _reAllocedPointers.at(i).allocFunc == reallocFunc) {
          _reAllocedPointers.at(i).reAllocedLine = reallocLine;
        }
      }
    }
  }

  virtual std::vector<StatementMatcher> getMatchers() override {
    return _matchers;
  }

  virtual void writeOutput() override {
    removeFromVector();
    for (ReAllocedPointer element : _reAllocedPointers) {
      llvm::outs() << "Pointer " << element.name
                   << " declared at: " << element.allocLine
                   << ", is hidden by a reallocation at: "
                   << element.reAllocedLine << " !\n";
    }
  }
};