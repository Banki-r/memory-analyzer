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
    datatypes/VisitedContainer.h
    datatypes/MemberLoc.h
    datatypes/ImplementedLinkedlist.h
    cases/matchers/MallocMatcher.cpp
    cases/matchers/NewMatcher.cpp
    cases/matchers/ReAllocMatcher.cpp
    cases/matchers/FunctionAllocMatcher.cpp
    cases/matchers/UninitializedMatcher.cpp
    cases/matchers/ContainerMatcher.cpp
    cases/matchers/LinkedlistImplMatcher.cpp
    cases/matchers/VectorImplMatcher.cpp
    cases/matchers/IMatcher.h
    cases/visitors/IASTVisitor.h
    cases/visitors/ContainerVisitor.cpp
)

target_link_libraries(memory-analyzer
    PRIVATE
    clangAST
    clangFrontend
    clangTooling
)