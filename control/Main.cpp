#include <gtkmm/application.h>

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "FrontendFactoryWrapper.h"
#include "view/MainWindow.h"

#include <memory>

using namespace clang::tooling;
using namespace llvm;

static llvm::cl::OptionCategory toolCategory("class-analyzer <options>");
int main(int argc, char **argv) {
  auto app = Gtk::Application::create("memory anaylzer");
  app->make_window_and_run<MainWindow>(argc, argv);


  /*
  auto argsParser = CommonOptionsParser::create(argc, const_cast<const char**>(argv), toolCategory);
  if (!argsParser) {
    llvm::errs() << argsParser.takeError();
    return -1;
  }
  CommonOptionsParser &optionsParser = argsParser.get();
  ClangTool tool(optionsParser.getCompilations(),
                 optionsParser.getSourcePathList());
  auto myActionFactory = std::make_unique<FrontendFactoryWrapper>();
  tool.run(myActionFactory.get());
  */
  return 0;
}
