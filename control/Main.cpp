#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "control/FrontendFactoryWrapper.h"
#include "view/MainWindow.h"
#include <memory>

using namespace clang::tooling;
using namespace llvm;
static llvm::cl::OptionCategory toolCategory("memory-analyzer <options>");
void run_clang(int argc, const char **argv)
{
  auto argsParser = CommonOptionsParser::create(argc, argv, toolCategory);
  if (!argsParser) {
  llvm::errs() << argsParser.takeError();
      return;
  }
  CommonOptionsParser &optionsParser = argsParser.get();
  ClangTool tool(optionsParser.getCompilations(),
                  optionsParser.getSourcePathList());
  auto myActionFactory = std::make_unique<FrontendFactoryWrapper>();
  tool.run(myActionFactory.get());
}

int main(int argc, char **argv) {
  void (*func)(int, const char**);
  func = run_clang;
  auto app = Gtk::Application::create("memory analyzer");
  app->make_window_and_run<MainWindow>(argc, argv, func, argc);

  return 0;
}
