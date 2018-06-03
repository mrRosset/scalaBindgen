#include "visitor/ScalaFrontendAction.h"
#include <sstream>
#include "Utils.h"
#include "visitor/ScalaFrontendActionFactory.h"
#include <clang/Tooling/CommonOptionsParser.h>


static llvm::cl::OptionCategory Category("Binding Generator");
static llvm::cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);
static llvm::cl::extrahelp MoreHelp("\nProduce Bindings for scala native. Please specify lib name wit parameter name\n");
static llvm::cl::opt<std::string> LibName("name", llvm::cl::cat(Category));
static llvm::cl::opt<std::string> StdHeaders("stdHeaders", llvm::cl::cat(Category));
static llvm::cl::opt<bool> PrintHeadersLocation ("location", llvm::cl::cat(Category));


int main(int argc, char *argv[]) {
    clang::tooling::CommonOptionsParser op(argc, (const char**)argv, Category);
    clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    auto libName = LibName.getValue();
    if(libName.empty()){
        llvm::errs() << "Error: Please specify the lib name using -name parameter\n";
        return -1;
    }

    auto stdhead = StdHeaders.getValue();
    if(!stdhead.empty()){
        headerMan.LoadConfig(stdhead);
    }

    locations.clear();

    ScalaFrontendActionFactory actionFactory(libName);

    int result = Tool.run(&actionFactory);

    IR ir = actionFactory.getIntermediateRepresentation();

    auto printLoc = PrintHeadersLocation.getValue();

    std::ostringstream s;

    if(printLoc){
        for(const auto& location: locations) {
            s << location.c_str();
        }
    } else {
        s << ir.generate();
    }
    std::cout << s.str();
    return result;
}
