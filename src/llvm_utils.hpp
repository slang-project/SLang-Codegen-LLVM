// LLVM INCLUDES
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"


// NAMESPACES SETUP
using namespace llvm;


// GLOBAL LLVM CONTAINERS

extern LLVMContext TheContext;
extern IRBuilder<> Builder;
extern std::unique_ptr<Module> TheModule;


// EXTERNAL INTERFACE

AllocaInst *CreateEntryBlockAlloca(Function* TheFunction, Type* type, const std::string &VarName);
bool isLibcName(const std::string &name);  // TODO: move away
void initLLVMGlobal(const std::string &moduleName);
bool generateStartupRoutine(const std::string &mainName); // TODO: move away
void addType(const std::string &name, Type *llvmType, Value *defValue);
Type *getLLVMType(const std::string &name);
Value *getLLVMDefaultValue(const std::string &name);
int createObjectFile(const std::string &outFilePath);
void writeGeneratedCode(const std::string &outFilePath);
