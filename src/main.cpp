#include "deserializer.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <fstream>

using json = nlohmann::json;

// given command line arguments try to deduce 
std::string get_filepath(const int &argc, char **argv)
{
    if (argc >= 2)
    {
        std::string filename{argv[1]};
        return filename;        
    }
    else
        return std::string{};
}

int main(int argc, char **argv)
{
    // TODO: Consider using Boost.Program_options for cli
    // Get filepath from command line args
    std::string input_filepath{get_filepath(argc, argv)};
    
    // Check file existance
    if (!input_filepath.length())
    {
        std::cerr << "No input file provided.\nUSAGE: " << "slang_jtll" << " <filepath>" << '\n';
        return 1;
    }

    // Read file by path into input stream
    std::fstream input_file{input_filepath, std::ios_base::in};
    
    if (!input_file)
    {
        std::cerr << "No input file found: " << input_filepath << std::endl;
        return 1;
    }

    // Parse json file
    json input = json::parse(input_file);

    initLLVMGlobal(input_filepath);

    CompilationAST* root = deserializeCompilationAST(input);
    root->codegen();

    printGeneratedCode("out.ll");
    return 0;
}
