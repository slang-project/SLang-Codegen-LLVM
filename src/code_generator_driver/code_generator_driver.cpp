#include "slang_codegen/code_generator_driver/code_generator_driver.hpp"

#include "ir_converter/ir_converter.hpp"
#include "object_file_generator/object_file_generator.hpp"

#include <fstream>
#include <system_error>

using namespace SLang::CodeGenerator;

using namespace std;

bool Driver::Main(
    const CommandLineArgs &args,
    ostream &outs,
    ostream &errs,
    ostream &logs) noexcept
{
    const auto &in_path = args.InputPath();
    if (error_code ec; !filesystem::exists(in_path, ec) || ec)
    {
        // TODO: message on file not found.
        return false;
    }

    // TODO: \c std::basic_ifstream<char8_t> (or \c std::u8ifstream if present).
    ifstream in_file(in_path);
    if (!in_file.is_open())
    {
        // TODO: message on file open failure.
        return false;
    }

    // TODO: move to \c noexcept version when it will be available in the library.
    const auto parsed_content = nlohmann::json::parse(in_file, nullptr, false);
    in_file.close();
    if (parsed_content.is_discarded())
    {
        // TODO: message on parsing failure.
        return false;
    }

    const auto program_module = Ir::Convert(parsed_content);
    if (!program_module)
    {
        // TODO: message on code conversion failure.
        return false;
    }

    if (!ObjectFileGenerator::ConvertIrToObjectFile(*program_module, args.OutputPath()))
    {
        // TODO: message on object file generation failure.
        return false;
    }

    return true;
}
