#pragma once

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/DeclCXX.h>
#include <vector>

class ASTVisitorWrapper : public clang::RecursiveASTVisitor<ASTVisitorWrapper>
{
private:
    clang::ASTContext *_context;
public:
    ASTVisitorWrapper(clang::ASTContext *context);
    std::string locStr(clang::SourceLocation loc);
    std::string declLocStr(clang::Decl const *decl);
    std::string typeStr(clang::QualType qType);
    bool VisitDeclaratorDecl(clang::DeclaratorDecl *record);
    bool VisitDeclRefExpr(clang::DeclRefExpr *refexpr);
    bool TraverseDecl(clang::Decl *decl);
};