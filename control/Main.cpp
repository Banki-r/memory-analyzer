#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include "FrontendFactoryWrapper.h"

using namespace clang::tooling;
using namespace llvm;

static llvm::cl::OptionCategory toolCategory("memory-analyzer <options>");

int main(int argc, const char** argv)
{
    auto argsParser = CommonOptionsParser::create(argc, argv, toolCategory);
    if(!argsParser)
    {
        llvm::errs() << argsParser.takeError();
        return -1;
    }

    llvm::outs() << "Memory analyzer started...\n";

    CommonOptionsParser& optionParser = argsParser.get();
    ClangTool tool(optionParser.getCompilations(), optionParser.getSourcePathList());

    auto actionFactory = std::make_unique<FrontendFactoryWrapper>();
    return tool.run(actionFactory.get());
}