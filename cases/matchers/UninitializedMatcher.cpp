#include "IMatcher.h"
#include <datatypes/UninitializedPointer.h>
#include <vector>

using namespace clang;
using namespace clang::ast_matchers;

class UninitializedMatcher : public IMatcher {
private:
  StatementMatcher _uMatcher =
      declStmt(hasDescendant(varDecl().bind("var"))).bind("line");

  std::vector<UninitializedPointer> _uninitializedPointers;
  std::vector<StatementMatcher> _matchers = {_uMatcher};

public:
  virtual void run(const MatchFinder::MatchResult &result) override {
    auto allocNode = result.Nodes.getNodeAs<DeclStmt>("line");
    auto allocVar = result.Nodes.getNodeAs<VarDecl>("var");

    if (allocNode && allocVar) {
      if (result.Context->getSourceManager().isWrittenInMainFile(allocNode->getBeginLoc()) &&
         allocVar->getType().getTypePtr()->isPointerType() && !allocVar->hasInit()) {
        UninitializedPointer up;
        up.allocLine = allocNode->getBeginLoc().printToString(
            result.Context->getSourceManager());
        up.name = allocVar->getNameAsString();
        _uninitializedPointers.push_back(up);
      }
    }
  }

  virtual std::vector<StatementMatcher> getMatchers() override {
    return _matchers;
  }

  virtual void writeOutput() override {
    for (UninitializedPointer element : _uninitializedPointers) {
      llvm::outs() << "Pointer " << element.name
                   << " is not initialized at declaration: " << element.allocLine << " !\n";
    }
  }
};