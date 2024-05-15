#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
using namespace llvm;

namespace
{

    struct InstrumentationPass : public PassInfoMixin<InstrumentationPass>
    {
        PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM)
        {
            LLVMContext &Context = M.getContext();
            IRBuilder<> Builder(Context);

            // Declare or insert the hooks into the module
            FunctionCallee onEnterFunc = M.getOrInsertFunction(
                "__Instr_onEnterFunc", FunctionType::getVoidTy(Context), Builder.getInt8PtrTy());
            FunctionCallee onArg = M.getOrInsertFunction(
                "__Instr_onArg", FunctionType::getVoidTy(Context), Builder.getInt8PtrTy(), Type::getInt32Ty(Context));
            FunctionCallee beforeExit = M.getOrInsertFunction(
                "__Instr_beforeExit", FunctionType::getVoidTy(Context), Builder.getInt8PtrTy(), Builder.getInt8PtrTy());

            // Iterate over all functions in the module
            for (Function &F : M)
            {
                if (F.isDeclaration() || F.getName().startswith("__Instr_"))
                    continue;
                else
                    errs() << "Instrumenting function: " << F.getName() << "\n";

                IRBuilder<> Builder(&*F.getEntryBlock().begin());
                Builder.CreateCall(onEnterFunc, {Builder.CreateGlobalStringPtr(F.getName())});

                // // Insert onArg for each pointer or struct argument
                unsigned ArgIndex = 0;
                for (Argument &Arg : F.args())
                {
                    if (Arg.getType()->isPointerTy() || Arg.getType()->isStructTy())
                    {
                        Builder.CreateCall(onArg, {Builder.CreateBitCast(&Arg, Type::getInt8PtrTy(Context)), Builder.getInt32(ArgIndex)});
                    }
                    ArgIndex++;
                }

                // Insert beforeExit before each return instruction
                for (BasicBlock &BB : F)
                {
                    for (Instruction &I : BB)
                    {
                        if (ReturnInst *RI = dyn_cast<ReturnInst>(&I))
                        {
                            Builder.SetInsertPoint(RI);

                            Value *returnValue = RI->getReturnValue();
                            Value *returnValuePtr = nullptr;

                            if (returnValue)
                            {
                                // Allocate space for the return value
                                AllocaInst *alloca = Builder.CreateAlloca(returnValue->getType());
                                Builder.CreateStore(returnValue, alloca);
                                returnValuePtr = alloca;
                            }
                            else
                            {
                                // If the function returns void, use a null pointer
                                returnValuePtr = ConstantPointerNull::get(Type::getInt8PtrTy(Context));
                            }

                            Builder.CreateCall(beforeExit, {Builder.CreateGlobalStringPtr(F.getName()),
                                                            returnValuePtr});
                        }
                    }
                }
            }
            return PreservedAnalyses::all();
        };
    };
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo()
{
    return {
        .APIVersion = LLVM_PLUGIN_API_VERSION,
        .PluginName = "Skeleton pass",
        .PluginVersion = "v0.1",
        .RegisterPassBuilderCallbacks = [](PassBuilder &PB)
        {
            PB.registerPipelineStartEPCallback(
                [](ModulePassManager &MPM, OptimizationLevel Level)
                {
                    MPM.addPass(InstrumentationPass());
                });
        }};
}
