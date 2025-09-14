#include "IASTVisitor.h"
#include "datatypes/MemberLoc.h"
#include "datatypes/VisitedContainer.h"
#include "clang/Basic/SourceManager.h"

class ContainerVisitor : public IASTVisitor<ContainerVisitor> {
private:
  std::vector<VisitedContainer> *_VisitedContainers;
  std::vector<MemberLoc> *_memberLocs;

public:
  ContainerVisitor(clang::ASTContext *context,
                   std::vector<VisitedContainer> *VisitedContainers,
                   std::vector<MemberLoc> *memberLocs) {
    _VisitedContainers = VisitedContainers;
    _memberLocs = memberLocs;
    _context = context;
  }

  bool isItAContainer(std::string str) {
    bool isVector = (str.find("std::vector") != std::string::npos ||
                     str.find("vector") != std::string::npos);
    bool isDeque = (str.find("std::deque") != std::string::npos ||
                    str.find("deque") != std::string::npos);
    bool isList = (str.find("std::list") != std::string::npos ||
                   str.find("list") != std::string::npos);

    return isVector || isDeque || isList;
  }

  // visits the alloc line
  bool VisitVarDecl(const clang::VarDecl *decl) {

    clang::QualType type = decl->getType();
    std::string typeAsString = type.getAsString();
    // check for std containers

    if (_context->getSourceManager().isWrittenInMainFile(decl->getBeginLoc()) &&
        isItAContainer(typeAsString)) {
      VisitedContainer vv;
      vv.objName = decl->getNameAsString();
      vv.allocedLine = locStr(decl->getBeginLoc());
      _VisitedContainers->push_back(vv);
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
    if (isItAContainer(declref->getType().getAsString())) {
      // check if there is a push_back or insert on a vector
      for (std::vector<MemberLoc>::iterator memit = _memberLocs->begin(),
                                            memend = _memberLocs->end();
           memit != memend; ++memit) {
        if (memit.base()->loc == locStr(declref->getBeginLoc())) {
          // check which vector that is
          for (std::vector<VisitedContainer>::iterator
                   vecit = _VisitedContainers->begin(),
                   vecend = _VisitedContainers->end();
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