
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "FrontendFactoryWrapper.h"

#include <memory>

using namespace clang::tooling;
using namespace llvm;

static llvm::cl::OptionCategory toolCategory("class-analyzer <options>");

int main(int argc, const char **argv) {
  auto argsParser = CommonOptionsParser::create(argc, argv, toolCategory);
  if (!argsParser) {
    llvm::errs() << argsParser.takeError();
    return -1;
  }
  CommonOptionsParser &optionsParser = argsParser.get();
  ClangTool tool(optionsParser.getCompilations(),
                 optionsParser.getSourcePathList());
  auto myActionFactory = std::make_unique<FrontendFactoryWrapper>();

  return tool.run(myActionFactory.get());
}
