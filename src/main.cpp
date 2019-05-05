#include "deserializer.hpp"

#include <nlohmann/json.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

using json = nlohmann::json;

struct Parameters
{
    std::string in_path {};  // TODO: filesystem::path

    std::string out_path { "app" };  // TODO: filesystem::path

    bool save_ll_listing = false;
    bool save_o_file = true;
};

// Given command line arguments try to deduce parameters
void parse_args(Parameters &params, const int &argc, const char * const * const &argv)
{
    if (argc >= 2)
    {
        params.in_path = std::string { argv[1] };
    }
}

int main(const int argc, const char * const * const argv)
{
    // TODO: Consider using Boost.Program_options for cli
    Parameters params; parse_args(params, argc, argv);

    const std::string &input_filepath = params.in_path;
    if (!input_filepath.length())
    {
        std::cerr << "No input file provided.\nUSAGE: " << "slang_jtll" << " <filepath>" << std::endl;
        return EXIT_FAILURE;
    }

    // Read file by path into input stream
    std::fstream input_file { input_filepath, std::ios_base::in };
    if (!input_file)
    {
        std::cerr << "Cannot open input file: " << input_filepath << std::endl;
        return EXIT_FAILURE;
    }

    // JSON PARSE
    json input = json::parse(input_file);

    // CODE GENERATION
    initLLVMGlobal(input_filepath);

    const CompilationAST* root = deserializeCompilationAST(input);

    if (!root->codegen())
    {
        std::cerr << "Compilation failed: " << input_filepath << std::endl;
        return EXIT_FAILURE;
    }

    // OUTPUT WRITING
    const std::string &output_filepath = params.out_path;
    if (!output_filepath.length())
    {
        std::cerr << "No output path provided." << std::endl;
        return EXIT_FAILURE;
    }

    // LL FILE WRITE
    if (params.save_ll_listing)
    {
        static const std::string ll_file_format { ".ll" };
        static const std::string ll_file_path { output_filepath + ll_file_format };  // TODO: filesystem::path
        writeGeneratedCode(ll_file_path);
    }

    // OBJECT FILE WRITE
    static const std::string o_file_format { ".o" };
    static const std::string o_file_path { output_filepath + o_file_format };  // TODO: filesystem::path
    createObjectFile(o_file_path);
/*
    // LINKAGE
    // Note: recommended command is...
    // ld -o app -dynamic-linker /lib/ld-linux.so.2 /usr/lib/crt1.o /usr/lib/crti.o app.o -lc /usr/lib/crtn.o
    static const std::string linkCall { "clang -o " + params.out_path + " " + o_file_path };
    const bool linkage_succeed = !std::system(linkCall.c_str());

    if (!params.save_o_file)
    {
        static const std::string rmCall { "rm " + o_file_path };
        std::system(rmCall.c_str());  // TODO: filesystem::remove
    }

    if (!linkage_succeed)
    {
        std::cerr << "Linkage failed: " << linkCall << std::endl;
        return EXIT_FAILURE;
    }
*/
    return EXIT_SUCCESS;
}
