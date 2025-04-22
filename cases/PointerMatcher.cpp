#include "IMatcher.h"
#include <vector>
#include <datatypes/allocedPointer.h>

class PointerMatcher : public IMatcher
{
private:
    std::vector<allocedPointer> _pointers;
public:
    PointerMatcher()
    {
        _matcher = declStmt(hasDescendent(callExpr(callee(funcionDecl(hasName("malloc"))))));
    }
};