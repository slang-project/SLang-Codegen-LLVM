#include "llvm_utils.hpp"

#include <algorithm>

LLVMContext TheContext;
IRBuilder<> Builder(TheContext);
std::unique_ptr<Module> TheModule;

static const std::string _exitName = "_Exit";

static std::map<const std::string, std::pair<Type*, Value*>> TypeTable
{
    { "Boolean",   { Type::getInt1Ty(TheContext),   ConstantInt::get(TheContext, APInt(1, 0, false)) } },  // TODO: remove
    { "Integer",   { Type::getInt16Ty(TheContext),  ConstantInt::get(TheContext, APInt(16, 0, true)) } },
    { "Real",      { Type::getDoubleTy(TheContext), ConstantFP::get(TheContext, APFloat(0.0))        } },
    { "Character", { Type::getInt8Ty(TheContext),   ConstantInt::get(TheContext, APInt(8, 0, false)) } },
    { "String",    { nullptr,                       nullptr                                          } },  // TODO

    { "c$int",     { Type::getInt16Ty(TheContext),  ConstantInt::get(TheContext, APInt(16, 0, true)) } },
};

static std::map<const std::string, FunctionType*> LibcInterfaces
{
    // ISO/IEC 9899:2018, 7.22.4.5 The _Exit function
    { _exitName, FunctionType::get(
            Type::getVoidTy(TheContext),
            std::vector<Type*> { getLLVMType("c$int") },
            false) },
};

bool isLibcName(const std::string &name)
{
    // Reserved identifiers and possible collisions with the C Standard Library
    // ISO/IEC 9899:2018, 7.1.3 Reserved identifiers
    // TODO: enumerations?
    static const std::vector<std::string> LibcNames
    {
        #include "libc_names.def"
    };
    return (name[0] == '_' && (isupper(name[1]) || name[1] == '_'))
        || std::find(LibcNames.begin(), LibcNames.end(), name) != LibcNames.end();
}

void initLLVMGlobal(const std::string &moduleName)
{
    TheModule = llvm::make_unique<Module>(moduleName, TheContext);

    // EXTERNAL DECLARATIONS
    for (const auto &libcEntryPair : LibcInterfaces)
    {
        Function::Create(
            libcEntryPair.second, // FunctionType
            Function::ExternalLinkage,
            libcEntryPair.first, // Name
            TheModule.get());
    }
}

bool generateStartupRoutine(const std::string &mainName)
{
    // Create startup function, TODO: move in future
    static const std::string _startName = "_start";
    FunctionType * const _startType = FunctionType::get(Type::getVoidTy(TheContext), false);
    Function * const _start = Function::Create(_startType, Function::ExternalLinkage, _startName, TheModule.get());
    BasicBlock * const BB = BasicBlock::Create(TheContext, _startName, _start);

    // Startup function body
    Function * const main = TheModule->getFunction(mainName);
    if (!main)
    {
        _start->eraseFromParent();
        return false;
    }

    Function * const _exit = TheModule->getFunction(_exitName);
    if (!_exit)
    {
        _start->eraseFromParent();
        return false;
    }

    static const std::vector<Value*> mainArgs {};
    Builder.SetInsertPoint(BB);
    Value * const mainRes = Builder.CreateCall(main, mainArgs, "maincall");
    Value * const castedRes = Builder.CreateIntCast(mainRes, LibcInterfaces[_exitName]->getParamType(0), true, "rescast");
    if (!Builder.CreateCall(_exit, castedRes))
    {
        _start->eraseFromParent();
        return false;
    }
    Builder.CreateUnreachable();
    if (verifyFunction(*_start, &errs()))
    {
        _start->eraseFromParent();
        return false;
    }
    return true;
}

void addType(const std::string &name, Type * const llvmType, Value * const defValue)
{
    throw std::string(__func__) + " not implemented yet";
    TypeTable[name] = std::pair<Type*, Value*> { llvmType, defValue };
}

Type *getLLVMType(const std::string &name)
{
    if (TypeTable.find(name) == TypeTable.end())
    {
        return nullptr;
    }
    return TypeTable[name].first;
}

Value *getLLVMDefaultValue(const std::string &name)
{
    if (TypeTable.find(name) == TypeTable.end())
    {
        return nullptr;
    }
    return TypeTable[name].second;
}

/// Write object file with given filename, return 0 indicates correct operation complete.
int createObjectFile(const std::string &outFilePath)
{
    const std::string TargetTriple = sys::getDefaultTargetTriple();
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();

    std::string Error;
    const auto trg = TargetRegistry::lookupTarget(TargetTriple, Error);

    static const std::string CPU = "generic";
    static const std::string Features = "";

    const TargetOptions opt {};
    const auto RM = Optional<Reloc::Model>();
    const auto TargetMachine = trg->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    TheModule->setDataLayout(TargetMachine->createDataLayout());
    TheModule->setTargetTriple(TargetTriple);

    std::error_code EC;
    raw_fd_ostream dest(outFilePath, EC, sys::fs::F_None);

    if (EC) {
        errs() << "Could not open file: " << EC.message();
        return 1;
    }
    legacy::PassManager pass {};
    const auto FileType = TargetMachine::CGFT_ObjectFile;

    if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        errs() << "TargetMachine can't emit a file of this type";
        return 1;
    }

    pass.run(*TheModule);
    dest.flush();
    return 0;
}

void writeGeneratedCode(const std::string &outFilePath)
{
    std::error_code EC;
    raw_fd_ostream file(outFilePath.c_str(), EC);
    TheModule->print(file, nullptr);
}
