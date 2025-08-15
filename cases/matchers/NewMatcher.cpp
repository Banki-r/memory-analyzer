#include "IMatcher.h"
#include <datatypes/CastedPointer.h>
#include <vector>

using namespace clang;
using namespace clang::ast_matchers;

class NewMatcher : public IMatcher {
private:
  StatementMatcher _nMatcher =
      declStmt(hasDescendant(varDecl(hasDescendant(cxxNewExpr())).bind("var")))
          .bind("new");
  StatementMatcher _dMatcher =
      cxxDeleteExpr(hasDescendant(implicitCastExpr(
                        hasDescendant(declRefExpr().bind("var")))))
          .bind("delete");
  StatementMatcher _castMatcher =
      declStmt(hasDescendant(varDecl(hasDescendant(implicitCastExpr(
                   hasDescendant(declRefExpr().bind("castedVar")))))))
          .bind("castedNode");
  StatementMatcher _retMatcher =
      returnStmt(hasDescendant(declRefExpr().bind("retVar"))).bind("return");

  std::vector<CastedPointer> _castedPointers;
  std::vector<StatementMatcher> _matchers = {_retMatcher, _nMatcher,
                                             _castMatcher, _dMatcher};

  void removeFromVector() {

    _castedPointers.erase(
        std::remove_if(_castedPointers.begin(), _castedPointers.end(),
                       [](CastedPointer i) {
                         return !i.freeLine.empty() && i.recastLine.empty();
                       }),
        _castedPointers.end());
  }

public:
  virtual void run(const MatchFinder::MatchResult &result) override {
    auto newNode = result.Nodes.getNodeAs<DeclStmt>("new");
    auto newVar = result.Nodes.getNodeAs<VarDecl>("var");

    auto deleteNode = result.Nodes.getNodeAs<CXXDeleteExpr>("delete");
    auto deleteVar = result.Nodes.getNodeAs<DeclRefExpr>("var");

    auto castedNode = result.Nodes.getNodeAs<DeclStmt>("castedNode");
    auto castedVar = result.Nodes.getNodeAs<DeclRefExpr>("castedVar");

    auto retVar = result.Nodes.getNodeAs<DeclRefExpr>("retVar");

    if (newNode && newVar) {
      if (result.Context->getSourceManager().isWrittenInMainFile(newNode->getBeginLoc())) {
        CastedPointer ap;
        ap.allocFunc = getParentFunction(result, *newNode)->getNameAsString();
        ap.allocLine = newNode->getBeginLoc().printToString(
          result.Context->getSourceManager());
      
        ap.name = newVar->getNameAsString();
        ap.freeLine = "";
        _castedPointers.push_back(ap);
      }
    }

    if (castedNode && castedVar) {
      std::string castedVarName =
          castedVar->getNameInfo().getName().getAsString();
      std::string recastedLine = castedNode->getBeginLoc().printToString(
          result.Context->getSourceManager());
      for (size_t i = 0; i < _castedPointers.size(); ++i) {
        if (_castedPointers.at(i).name == castedVarName) {
          _castedPointers.at(i).recastLine = recastedLine;
        }
      }
    }

    if (retVar) {
      auto retVarName = retVar->getNameInfo().getAsString();
      for (size_t i = 0; i < _castedPointers.size(); ++i) {
        if (_castedPointers.at(i).name == retVarName) {
          _castedPointers.at(i).freeLine = "returned by function";
        }
      }
    }

    if (deleteNode) {
      std::string deleteFunc =
          getParentFunction(result, *deleteNode)->getNameAsString();
      std::string varName = deleteVar->getNameInfo().getAsString();
      std::string freeLine = deleteNode->getBeginLoc().printToString(
          result.Context->getSourceManager());

      for (size_t i = 0; i < _castedPointers.size(); ++i) {
        if (_castedPointers.at(i).name == varName &&
            _castedPointers.at(i).allocFunc == deleteFunc) {
          _castedPointers.at(i).freeLine = freeLine;
        }
      }
    }
  }

  virtual std::vector<StatementMatcher> getMatchers() override {
    return _matchers;
  }

  virtual void writeOutput() override {
    removeFromVector();
    for (CastedPointer element : _castedPointers) {
      if (element.freeLine.empty()) {
        llvm::outs() << "Variable " << element.name
                     << " declared with new call at: " << element.allocLine
                     << ", is not deleted!\n";
      }
      if (!element.recastLine.empty()) {
        llvm::outs() << "Variable " << element.name
                     << " declared with new call at: " << element.allocLine
                     << ", is recasted at: " << element.recastLine << "!\n";
      }
    }
  }
};