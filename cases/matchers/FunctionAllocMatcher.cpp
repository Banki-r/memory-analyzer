#include "IMatcher.h"
#include <datatypes/AllocedPointer.h>
#include <vector>

using namespace clang;
using namespace clang::ast_matchers;

class FunctionAllocMatcher : public IMatcher {
private:
  StatementMatcher _aMatcher =
      declStmt(hasDescendant(varDecl().bind("var")),
               hasDescendant(callExpr(callee(functionDecl().bind("function")))))
          .bind("allocNode");
  StatementMatcher _dMatcher =
      cxxDeleteExpr(hasDescendant(implicitCastExpr(
                        hasDescendant(declRefExpr().bind("var")))))
          .bind("delete");
  StatementMatcher _retMatcher =
      returnStmt(hasDescendant(declRefExpr().bind("retVar"))).bind("return");

  std::vector<AllocedPointer> _allocedPointers;
  std::vector<StatementMatcher> _matchers = {_retMatcher, _aMatcher, _dMatcher};

  void removeFromVector() {

    _allocedPointers.erase(
        std::remove_if(_allocedPointers.begin(), _allocedPointers.end(),
                       [](AllocedPointer i) { return !i.freeLine.empty(); }),
        _allocedPointers.end());
  }

public:
  virtual void run(const MatchFinder::MatchResult &result) override {
    auto allocNode = result.Nodes.getNodeAs<DeclStmt>("allocNode");
    auto varNode = result.Nodes.getNodeAs<VarDecl>("var");
    auto functionNode = result.Nodes.getNodeAs<FunctionDecl>("function");

    auto deleteNode = result.Nodes.getNodeAs<CXXDeleteExpr>("delete");
    auto deleteVar = result.Nodes.getNodeAs<DeclRefExpr>("var");

    auto retVar = result.Nodes.getNodeAs<DeclRefExpr>("retVar");

    if (allocNode && varNode && functionNode &&
        functionNode->getNameInfo().getAsString().find("malloc") ==
            std::string::npos) {
      _allocFunc = getParentFunction(result, *allocNode);
      AllocedPointer ap;
      ap.allocLine = allocNode->getBeginLoc().printToString(
          result.Context->getSourceManager());
      if (ap.allocLine.find(".cpp") != std::string::npos) {
        ap.name = varNode->getNameAsString();
        ap.freeLine = "";
        _allocedPointers.push_back(ap);
      }
    }

    if (retVar) {
      auto retVarName = retVar->getNameInfo().getAsString();
      for (size_t i = 0; i < _allocedPointers.size(); ++i) {
        if (_allocedPointers.at(i).name == retVarName) {
          _allocedPointers.at(i).freeLine = "returned by function";
        }
      }
    }

    if (deleteNode) {
      _reallocFunc = getParentFunction(result, *deleteNode);
      if (_allocFunc == _reallocFunc) {
        std::string varName = deleteVar->getNameInfo().getAsString();
        std::string freeLine = deleteNode->getBeginLoc().printToString(
            result.Context->getSourceManager());

        for (size_t i = 0; i < _allocedPointers.size(); ++i) {
          if (_allocedPointers.at(i).name == varName) {
            _allocedPointers.at(i).freeLine = freeLine;
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
    for (AllocedPointer element : _allocedPointers) {
      llvm::outs() << "Variable " << element.name
                   << " declared with a function call at: " << element.allocLine
                   << ", is not deleted!\n";
    }
  }
};