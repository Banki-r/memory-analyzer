#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "control/FrontendFactoryWrapper.h"
#include "view/MainWindow.h"
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

using namespace clang::tooling;
using namespace llvm;
static llvm::cl::OptionCategory toolCategory("memory-analyzer <options>");

std::string Output;

std::string run_clang(std::string *filePath) {
  std::vector<std::string> sourceFiles = {*filePath};
  std::string workingDir = std::filesystem::current_path().generic_string();
  std::vector<std::string> compArgs = {
      "-Xclang",
      "-ast-dump",
      "-fsyntax-only",
      "-I/usr/lib/clang/18/include",
  };
  FixedCompilationDatabase compilations(workingDir, compArgs);
  ClangTool tool(compilations, sourceFiles);
  auto myActionFactory = std::make_unique<FrontendFactoryWrapper>(&Output);
  tool.run(myActionFactory.get());

  return Output;
}

int main(int argc, char **argv) {
  std::string (*func)(std::string *);
  func = run_clang;
  auto app = Gtk::Application::create("memory analyzer");
  app->make_window_and_run<MainWindow>(argc, argv, func);

  return 0;
}
