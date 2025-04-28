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
    parser/ASTVisitorWrapper.h
    parser/ASTVisitorWrapper.cpp
    datatypes/allocedPointer.h
    cases/MallocMatcher.cpp
)

target_link_libraries(memory-analyzer
    PRIVATE
    clangAST
    clangFrontend
    clangTooling
)