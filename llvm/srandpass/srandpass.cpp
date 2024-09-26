// srandpass.cpp - 

#include "llvm/IR/User.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

namespace llvm {

class SrandPass : public PassInfoMixin<SrandPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM);
  
  static void registerCallbacks(PassBuilder &PB); 

private:
  void instrument(llvm::Function &F,
                  llvm::Function *EnterFn,
                  llvm::Function *ExitFn);
};
} // namespace

PreservedAnalyses SrandPass::run(Module &M, 
                                 ModuleAnalysisManager &MAM) {
  Function *F = M.getFunction("srand");
  if (F) {       
    for (auto &F : M.functions()) {
      for (auto &BB : F) {
        for (auto &I : BB) {
          CallInst *CI = dyn_cast<CallInst>(&I);
          if (nullptr == CI) 
            continue;
            
          Function *CF = CI->getCalledFunction();
          if (nullptr == CF)
            continue;
                        
          if (0 == CF->getName().compare("srand")) {
            Value *V = CI->getOperand(0);
            Value *NV =  ConstantInt::get(V->getType(), 1234567, false);
            CI->setArgOperand(0, NV);
            return PreservedAnalyses::none();
          }
        }
      } 
    }

    return PreservedAnalyses::all();
  }

return PreservedAnalyses::all();
}

void SrandPass::registerCallbacks(PassBuilder &PB) {  
  
  PB.registerPipelineParsingCallback(
    [](StringRef Name, ModulePassManager &PM,
      ArrayRef<PassBuilder::PipelineElement> InnerPipeline) -> bool {
      if (Name == "srandpass") {
        PM.addPass(SrandPass());
        return true;
      }
      return false;
    });
    
  PB.registerPipelineStartEPCallback(
  [](ModulePassManager &MPM, OptimizationLevel Level) {
    MPM.addPass(SrandPass());
  });
}    

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "srandpass", "v0.1", 
      SrandPass::registerCallbacks};
}
