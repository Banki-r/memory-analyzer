#pragma once

#include <clang/AST/DeclCXX.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <vector>

template <typename T> class IASTVisitor : public clang::RecursiveASTVisitor<T> {
public:
  clang::ASTContext *_context;

  std::string locStr(clang::SourceLocation loc) {
    return loc.printToString(_context->getSourceManager());
  }

  std::string declLocStr(clang::Decl const *decl) {
    return locStr(decl->getLocation());
  }

  std::string typeStr(clang::QualType qType) { return qType.getAsString(); }
};