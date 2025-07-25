#pragma once

#include <clang/AST/DeclCXX.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <vector>

class IASTVisitor : public clang::RecursiveASTVisitor<IASTVisitor> {
private:
  clang::ASTContext *_context;

public:
  std::string locStr(clang::SourceLocation loc) {
    return loc.printToString(_context->getSourceManager());
  }

  std::string declLocStr(clang::Decl const *decl) {
    return locStr(decl->getLocation());
  }

  std::string typeStr(clang::QualType qType) { return qType.getAsString(); }
};