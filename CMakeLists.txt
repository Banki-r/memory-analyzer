set(LLVM_LINK_COMPONENTS support)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

add_clang_executable(memory-analyzer
    control/FrontendActionWrapper.cpp
    control/FrontendActionWrapper.h
    control/FrontendFactoryWrapper.h
    control/FrontendFactoryWrapper.cpp
    control/Main.cpp
    parser/ASTConsumerWrapper.h
    parser/ASTConsumerWrapper.cpp
    datatypes/AllocedPointer.h
    datatypes/ReAllocedPointer.h
    datatypes/CastedPointer.h
    datatypes/UninitializedPointer.h
    datatypes/VisitedVector.h
    datatypes/MemberLoc.h
    cases/matchers/MallocMatcher.cpp
    cases/matchers/NewMatcher.cpp
    cases/matchers/ReAllocMatcher.cpp
    cases/matchers/FunctionAllocMatcher.cpp
    cases/matchers/UninitializedMatcher.cpp
    cases/matchers/VectorMatcher.cpp
    cases/matchers/IMatcher.h
    cases/visitors/IASTVisitor.h
    cases/visitors/VectorVisitor.cpp
)

target_link_libraries(memory-analyzer
    PRIVATE
    clangAST
    clangFrontend
    clangTooling
)