// seedpass.cpp - 

#include "llvm/IR/User.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Demangle/Demangle.h"
#include "llvm/Demangle/StringViewExtras.h"

using namespace llvm;

// From llvm-c++filt.cpp
static bool ParseParams;
static bool StripUnderscore;
static bool Types;

static std::string demangle(const std::string &Mangled)
{
  using llvm::itanium_demangle::starts_with;
  std::string_view DecoratedStr = Mangled;
  bool CanHaveLeadingDot = true;
  if (StripUnderscore && DecoratedStr[0] == '_')
  {
    DecoratedStr.remove_prefix(1);
    CanHaveLeadingDot = false;
  }

  std::string Result;
  if (nonMicrosoftDemangle(DecoratedStr, Result, CanHaveLeadingDot,
                           ParseParams))
    return Result;

  std::string Prefix;
  char *Undecorated = nullptr;

  if (Types)
    Undecorated = itaniumDemangle(DecoratedStr, ParseParams);

  if (!Undecorated && starts_with(DecoratedStr, "__imp_"))
  {
    Prefix = "import thunk for ";
    Undecorated = itaniumDemangle(DecoratedStr.substr(6), ParseParams);
  }

  Result = Undecorated ? Prefix + Undecorated : Mangled;
  free(Undecorated);
  return Result;
}

namespace llvm {

class SeedPass : public PassInfoMixin<SeedPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM);
  
  static void registerCallbacks(PassBuilder &PB); 

private:
  void instrument(llvm::Function &F,
                  llvm::Function *EnterFn,
                  llvm::Function *ExitFn);
};
} // namespace

PreservedAnalyses SeedPass::run(Module &M, 
                                 ModuleAnalysisManager &MAM) {  
  for (auto &F : M.functions()) {
    for (auto &BB : F) {
      for (auto &I : BB) {
        CallInst *CI = dyn_cast<CallInst>(&I);
        if (nullptr == CI) 
          continue;
        
        Function *CF = CI->getCalledFunction();
        if (nullptr == CF)
          continue;

        std::string FN = demangle(CF->getName());
        if (FN.ends_with("::seed(unsigned long)")) {
          outs() << CF->getName() << " => " << FN << "\n";
          Value *V = CI->getOperand(1);
          Value *NV =  ConstantInt::get(V->getType(), 1234567, false);
          CI->setArgOperand(1, NV);
        }
      }
    } 
  }

  return PreservedAnalyses::all();
}

void SeedPass::registerCallbacks(PassBuilder &PB) {  
  
  PB.registerPipelineParsingCallback(
    [](StringRef Name, ModulePassManager &PM,
      ArrayRef<PassBuilder::PipelineElement> InnerPipeline) -> bool {
      if (Name == "seedpass") {
        PM.addPass(SeedPass());
        return true;
      }
      return false;
    });
    
  PB.registerPipelineStartEPCallback(
  [](ModulePassManager &MPM, OptimizationLevel Level) {
    MPM.addPass(SeedPass());
  });
}    

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "seedpass", "v0.1", 
      SeedPass::registerCallbacks};
}
