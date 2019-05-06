#include "deserializer.hpp"

#include <nlohmann/json.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

using json = nlohmann::json;

struct Parameters
{
    std::string inPath {};  // TODO: filesystem::path

    std::string outPath { "app" };  // TODO: filesystem::path

    CompilationAST::StartupBehavior startupBehavior = CompilationAST::StartupBehavior::isStartup;

    bool saveLLlisting = false;
    // bool saveObjFile = true;
};

// Given command line arguments try to deduce parameters
void parseArgs(Parameters &params, const int &argc, const char * const * const &argv)
{
    if (argc >= 2)
    {
        params.inPath = std::string { argv[1] };
    }
}

int main(const int argc, const char * const * const argv)
{
    // TODO: Consider using Boost.Program_options for cli
    Parameters params; parseArgs(params, argc, argv);

    const std::string &inputFilepath = params.inPath;
    if (!inputFilepath.length())
    {
        std::cerr << "No input file provided.\nUSAGE: " << argv[0] << " <filepath>" << std::endl;
        return EXIT_FAILURE;
    }

    // Read file by path into input stream
    std::fstream inputFile { inputFilepath, std::ios_base::in };
    if (!inputFile)
    {
        std::cerr << "Cannot open input file: " << inputFilepath << std::endl;
        return EXIT_FAILURE;
    }

    // JSON PARSE
    json input = json::parse(inputFile);

    // CODE GENERATION
    initLLVMGlobal(inputFilepath);

    const CompilationAST* root = deserializeCompilationAST(input);

    if (!root->codegen(params.startupBehavior))
    {
        std::cerr << "Compilation failed: " << inputFilepath << std::endl;
        return EXIT_FAILURE;
    }

    // OUTPUT WRITING
    const std::string &outputFilePath = params.outPath;
    if (!outputFilePath.length())
    {
        std::cerr << "No output path provided." << std::endl;
        return EXIT_FAILURE;
    }

    // LL FILE WRITE
    if (params.saveLLlisting)
    {
        static const std::string llFileFormat { ".ll" };
        static const std::string llFilePath { outputFilePath + llFileFormat };  // TODO: filesystem::path
        writeGeneratedCode(llFilePath);
    }

    // OBJECT FILE WRITE
    static const std::string objFileFormat { ".o" };
    static const std::string objFilePath { outputFilePath + objFileFormat };  // TODO: filesystem::path
    createObjectFile(objFilePath);
/*
    // LINKAGE
    // Note: recommended command is...
    // ld -o app -dynamic-linker /lib/ld-linux.so.2 /usr/lib/crt1.o /usr/lib/crti.o app.o -lc /usr/lib/crtn.o
    static const std::string linkCall { "clang -o " + params.outPath + " " + objFilePath };
    const bool linkageSucceed = !std::system(linkCall.c_str());

    if (!params.saveObjFile)
    {
        static const std::string rmCall { "rm " + objFilePath };
        std::system(rmCall.c_str());  // TODO: filesystem::remove
    }

    if (!linkageSucceed)
    {
        std::cerr << "Linkage failed: " << linkCall << std::endl;
        return EXIT_FAILURE;
    }
*/
    return EXIT_SUCCESS;
}
