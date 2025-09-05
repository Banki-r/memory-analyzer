#include "IASTVisitor.h"
#include "datatypes/MemberLoc.h"
#include "datatypes/VisitedVector.h"
#include "clang/Basic/SourceManager.h"

class VectorVisitor : public IASTVisitor<VectorVisitor> {
private:
  std::vector<VisitedVector> visitedVectors;
  std::vector<MemberLoc> memberLocs;

public:
  VectorVisitor(clang::ASTContext *context) { _context = context; }

  // visits the alloc line
  bool VisitVarDecl(const clang::VarDecl *decl) {

    clang::QualType type = decl->getType();
    std::string typeAsString = type.getAsString();
    if (_context->getSourceManager().isWrittenInMainFile(decl->getBeginLoc()) &&
        (typeAsString.find("std::vector") != std::string::npos ||
         typeAsString.find("vector") != std::string::npos)) {
      VisitedVector vv;
      vv.objName = decl->getNameAsString();
      vv.allocedLine = locStr(decl->getBeginLoc());
      visitedVectors.push_back(vv);
    }
    return true;
  }

  // visits push_backs and inserts
  bool VisitMemberExpr(const clang::MemberExpr *memberExpr) {
    std::string funcName = memberExpr->getMemberNameInfo().getAsString();
    if (funcName == "push_back" || funcName == "insert") {
      MemberLoc ml;
      ml.loc = locStr(memberExpr->getBeginLoc());
      ml.deletable = false;
      memberLocs.push_back(ml);
    }
    return true;
  }

  bool VisitDeclRefExpr(const clang::DeclRefExpr *declref) {
    if (declref->getType().getAsString().find("std::vector") !=
        std::string::npos) {
      // check if there is a push_back or insert on a vector
      for (auto ins : memberLocs) {
        if (ins.loc == locStr(declref->getBeginLoc())) {
          // check which vector that is
          for (auto vector : visitedVectors) {
            if (vector.objName == declref->getNameInfo().getAsString()) {
              // increment the number of push_back or insert
              // and prepare for deletion of MemberLoc
              vector.push_backs += 1;
              ins.deletable = true;
            }
          }
        }
      }

      // delete the found memberLoc from the vector
      memberLocs.erase(std::remove_if(memberLocs.begin(), memberLocs.end(),
                                      [](MemberLoc i) { return i.deletable; }),
                       memberLocs.end());
    }
    return true;
  }

  void printNotDeletedVectors() {
    for (auto i : visitedVectors) {
      llvm::outs() << i.objName << " vector at " << i.allocedLine
                   << ", is not deleted properly.\n";
    }
  }
};