#pragma once

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/DeclCXX.h>
#include <vector>

class IASTVisitor : public clang::RecursiveASTVisitor<IASTVisitor>
{
private:
    clang::ASTContext *_context;
public:
    std::string locStr(clang::SourceLocation loc)
    {
        return loc.printToString(_context->getSourceManager());
    }

    std::string declLocStr(clang::Decl const *decl)
    {
        return locStr(decl->getLocation());
    }

    std::string ypeStr(clang::QualType qType)
    {
        return qType.getAsString();
    }
    virtual bool VisitDeclaratorDecl(clang::DeclaratorDecl *record) = 0;
    virtual bool VisitDeclRefExpr(clang::DeclRefExpr *refexpr) = 0;
    virtual bool TraverseDecl(clang::Decl *decl) = 0;
};