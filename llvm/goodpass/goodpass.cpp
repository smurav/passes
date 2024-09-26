// goodpass.cpp - 

#include "llvm/IR/User.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

namespace llvm {

class GoodPass : public PassInfoMixin<GoodPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM);
  
  static void registerCallbacks(PassBuilder &PB); 

private:
  void instrument(llvm::Function &F,
                  llvm::Function *EnterFn,
                  llvm::Function *ExitFn);
};
} // namespace

PreservedAnalyses GoodPass::run(Module &M, 
                                 ModuleAnalysisManager &MAM) {
  outs() << "It's a good pass\n";
  return PreservedAnalyses::all();
}

void GoodPass::registerCallbacks(PassBuilder &PB) {  
  
  PB.registerPipelineParsingCallback(
    [](StringRef Name, ModulePassManager &PM,
      ArrayRef<PassBuilder::PipelineElement> InnerPipeline) -> bool {
      if (Name == "goodpass") {
        PM.addPass(GoodPass());
        return true;
      }
      return false;
    });
    
  PB.registerPipelineStartEPCallback(
  [](ModulePassManager &MPM, OptimizationLevel Level) {
    MPM.addPass(GoodPass());
  });
}    

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "goodpass", "v0.1", 
      GoodPass::registerCallbacks};
}
