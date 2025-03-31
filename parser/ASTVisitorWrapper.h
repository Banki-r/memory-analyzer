#pragma once

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/DeclCXX.h>

class ASTVisitorWrapper : public clang::RecursiveASTVisitor<ASTVisitorWrapper>
{
private:
clang::ASTContext* Context;
public:
    explicit ASTVisitorWrapper(clang::ASTContext* Context) : Context(Context){};
    bool VisitCXXRecordDecl(clang::CXXRecordDecl* decl);
    bool VisitStmt(clang::Stmt* stmt);
    bool VisitDecl(clang::Decl* decl);
    bool VisitType(clang::Type* type);
};