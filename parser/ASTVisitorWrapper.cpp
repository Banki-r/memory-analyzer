#include "ASTVisitorWrapper.h"

bool ASTVisitorWrapper::isFirstLetterUpperCase(const std::string &str) {
    return str.size() != 0 && std::isupper(str[0]);
}

bool ASTVisitorWrapper::VisitCXXRecordDecl(const clang::RecordDecl *record) {
    std::string name = record->getNameAsString();

    if (!isFirstLetterUpperCase(name)) {
        llvm::outs() << "Record Decl : " << name
                  <<" doesn't start with uppercase! \n";
    }

    return true;
}

bool ASTVisitorWrapper::TraverseDecl(clang::Decl *decl)  {
    return
       clang::RecursiveASTVisitor<ASTVisitorWrapper>::TraverseDecl(decl);
}