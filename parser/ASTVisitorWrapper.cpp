#include "ASTVisitorWrapper.h"
#include "clang/AST/Decl.h"
#include "clang/AST/StmtCXX.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include <string>

ASTVisitorWrapper::ASTVisitorWrapper(clang::ASTContext *context)
{
    _context = context;
}

std::string ASTVisitorWrapper::locStr(clang::SourceLocation loc)
{
    return loc.printToString(_context->getSourceManager());
}

std::string ASTVisitorWrapper::declLocStr(clang::Decl const *decl)
{
    return locStr(decl->getLocation());
}

std::string ASTVisitorWrapper::typeStr(clang::QualType qType)
{
    return qType.getAsString();
}

bool ASTVisitorWrapper::VisitDeclaratorDecl(clang::DeclaratorDecl *record)
{
    clang::SourceLocation startLoc = record->getTypeSpecStartLoc();
    std::string varName = record->getQualifiedNameAsString();
    std::string varType = typeStr(record->getType());
    /*
    if(varType.find("*") != std::string::npos)
    {
        FoundPointer(locStr(startLoc), varName);
    }*/
    
    // clang::QualType qtype = _context->getTypeDeclType(record);
    // const clang::Type *type = qtype.getTypePtrOrNull();
    //llvm::outs() << "Record Decl : " << varName
    //        <<" is something! " << varType  << " start loc:" << locStr(startLoc)
    //        << "\n";
    return true;
}

bool ASTVisitorWrapper::VisitDeclRefExpr(clang::DeclRefExpr *refexpr)
{
    clang::SourceLocation startLoc = refexpr->getLocation();
    clang::ValueDecl *dec = refexpr->getDecl();
    std::string decName = dec->getQualifiedNameAsString();
    
    if(decName.find("malloc") != std::string::npos)
    {
        std::string decLoc = locStr(dec->getBeginLoc());
        llvm::outs() << "DeclRefExpr start loc:" << locStr(startLoc) << " with decl: " <<
         decName << "    decl loc: " << decLoc <<"\n";
        
    }else if(decName.find("free") != std::string::npos)
    {
        llvm::outs() << "DeclRefExpr start loc:" << locStr(startLoc) << " with decl: " << decName <<"\n";
    }
    
    return true;
}

bool ASTVisitorWrapper::TraverseDecl(clang::Decl *decl)
{
    return
       clang::RecursiveASTVisitor<ASTVisitorWrapper>::TraverseDecl(decl);
}

void ASTVisitorWrapper::FoundPointer(std::string allocLoc, std::string varName)
{
    llvm::outs() << "Pushing back " << varName << "at :" << allocLoc << "\n";
    AllocedPointer ap;
    ap.allocLine = allocLoc;
    ap.name = varName;
    _allocedPointers.push_back(ap);
}