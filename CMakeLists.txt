cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

set(HAVE_CSS_ATOMICS64_WITHOUT_LIB True)
set(HAVE_CSS_ATOMICS_WITHOUT_LIB True)

set(LLVM_LINK_COMPONENTS support)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INLCUDE_DIRS})

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

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
    view/MainWindow.h
    view/MainWindow.cpp

    tests/MallocMatcherTests.cc
)

enable_testing()

include(GoogleTest)

target_link_libraries(memory-analyzer
    PUBLIC
    ${GTKMM_LIBRARIES}
    clangAST
    clangFrontend
    clangTooling
)

add_definitions(-frtti)