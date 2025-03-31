#include "ASTVisitorWrapper.h"

bool ASTVisitorWrapper::VisitCXXRecordDecl(clang::CXXRecordDecl* decl)
{

    llvm::outs() << "got here cxx\n";
    const clang::Type* type = decl->getTypeForDecl();
    bool exit = type->isPointerType();

    if(exit)
    {
        llvm::outs() << "Pointer found: \n" << decl->getNameAsString();
    }

    
    return !exit;
}

bool ASTVisitorWrapper::VisitStmt(clang::Stmt* stmt)
{
    //logic
    llvm::outs() << "got here visit stmt";
    return true;
}

bool ASTVisitorWrapper::VisitDecl(clang::Decl* decl)
{
    //logic
    llvm::outs() << "got here visit decl " << decl->getDeclKindName();
    return true;
}

bool ASTVisitorWrapper::VisitType(clang::Type* type)
{
    //logic
    llvm::outs() << "got here visit type";
    return true;
}