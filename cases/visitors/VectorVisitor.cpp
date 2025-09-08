#include "IASTVisitor.h"
#include "datatypes/MemberLoc.h"
#include "datatypes/VisitedVector.h"
#include "clang/Basic/SourceManager.h"

class VectorVisitor : public IASTVisitor<VectorVisitor> {
private:
  std::vector<VisitedVector> *_visitedVectors;
  std::vector<MemberLoc> *_memberLocs;

public:
  VectorVisitor(clang::ASTContext *context,
                std::vector<VisitedVector> *visitedVectors,
                std::vector<MemberLoc> *memberLocs) {
    _visitedVectors = visitedVectors;
    _memberLocs = memberLocs;
    _context = context;
  }

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
      _visitedVectors->push_back(vv);
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
      _memberLocs->push_back(ml);
    }
    return true;
  }

  bool VisitDeclRefExpr(const clang::DeclRefExpr *declref) {
    if (declref->getType().getAsString().find("std::vector") !=
        std::string::npos) {
      // check if there is a push_back or insert on a vector
      for (std::vector<MemberLoc>::iterator memit = _memberLocs->begin(),
                                            memend = _memberLocs->end();
           memit != memend; ++memit) {
        if (memit.base()->loc == locStr(declref->getBeginLoc())) {
          // check which vector that is
          for (std::vector<VisitedVector>::iterator
                   vecit = _visitedVectors->begin(),
                   vecend = _visitedVectors->end();
               vecit != vecend; ++vecit) {
            if (vecit.base()->objName == declref->getNameInfo().getAsString()) {
              // increment the number of push_back or insert
              // and prepare for deletion of MemberLoc
              vecit.base()->push_backs += 1;
              memit.base()->deletable = true;
            }
          }
        }
      }

      // delete the found memberLoc from the vector
      _memberLocs->erase(
          std::remove_if(_memberLocs->begin(), _memberLocs->end(),
                         [](MemberLoc i) { return i.deletable; }),
          _memberLocs->end());
    }
    return true;
  }
};