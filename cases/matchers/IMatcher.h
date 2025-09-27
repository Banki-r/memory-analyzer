#pragma once

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"
#include <algorithm>
#include <string>

using namespace clang;
using namespace clang::ast_matchers;

class IMatcher : public MatchFinder::MatchCallback {
protected:
  template <typename T>
  const FunctionDecl *getParentFunction(const MatchFinder::MatchResult &result,
                                        T &Node) {
    clang::DynTypedNodeList parentNodeList = result.Context->getParents(Node);
    clang::DynTypedNode parentNode;
    while (!parentNodeList.empty()) {
      parentNode = parentNodeList[0];
      if (const FunctionDecl *Parent = parentNode.get<FunctionDecl>()) {
        return Parent;
      }
      parentNodeList = result.Context->getParents(parentNode);
    }
    return nullptr;
  }
  static bool caseInsensitiveCharCompare(char a, char b) {
    return tolower(a) == tolower(b);
  }

  bool caseInsensitiveSubstrSearch(const std::string &str,
                                   const std::string &substr) {
    auto it = std::search(str.begin(), str.end(), substr.begin(), substr.end(),
                          &caseInsensitiveCharCompare);

    llvm::outs() << (it != str.end()) << " is search\n";
    return it != str.end();
  }

public:
  virtual void run(const MatchFinder::MatchResult &result) override = 0;
  virtual std::vector<StatementMatcher> getMatchers() = 0;
  virtual void writeOutput() = 0;
};