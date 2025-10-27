#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "control/FrontendFactoryWrapper.h"
#include "view/MainWindow.h"
#include <memory>
#include <vector>
#include <filesystem>
#include <string>

using namespace clang::tooling;
using namespace llvm;
static llvm::cl::OptionCategory toolCategory("memory-analyzer <options>");
void run_clang(std::string *filePath)
{
  std::vector<std::string> sourceFiles = {*filePath};
  std::string workingDir = std::filesystem::current_path().generic_string();
  std::vector<std::string> compArgs = {
    "-Xclang",
    "-ast-dump",
    "-fsyntax-only",
    "-I/usr/lib/clang/18/include"
  };
  FixedCompilationDatabase compilations(workingDir, compArgs);
  ClangTool tool(compilations, sourceFiles);
  auto myActionFactory = std::make_unique<FrontendFactoryWrapper>();
  tool.run(myActionFactory.get());
}

int main(int argc, char **argv) {
  void (*func)(std::string *);
  func = run_clang;
  auto app = Gtk::Application::create("memory analyzer");
  app->make_window_and_run<MainWindow>(argc, argv, func);

  return 0;
}
