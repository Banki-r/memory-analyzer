#pragma once

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/DeclCXX.h>

class ASTVisitorWrapper : public clang::RecursiveASTVisitor<ASTVisitorWrapper> {
    public:

    bool isFirstLetterUpperCase(const std::string &str);
    bool VisitCXXRecordDecl(const clang::RecordDecl *record);
    bool TraverseDecl(clang::Decl *decl);
};