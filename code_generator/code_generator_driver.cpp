#include "code_generator_driver.hpp"

#include "ir_converter/ir_converter.hpp"
#include "object_file_generator/object_file_generator.hpp"

#include <fstream>

using namespace SLang::CodeGenerator;

using namespace std;

bool Driver::Main(
    const CommandLineArgs &args,
    wostream &outs,
    wostream &errs,
    wostream &logs) noexcept
{
    const auto &in_path = args.InputPath();
    if (error_code ec; !filesystem::exists(in_path, ec) || ec)
    {
        // TODO: message on file not found.
        return false;
    }

    // TODO: move to `std::wifstream` (`std::istream` is required by `nlohmann::json` as for 3.7.3).
    ifstream in_file(in_path);
    if (!in_file.is_open())
    {
        // TODO: message on file open failure.
        return false;
    }

    // TODO: move to `noexcept` version when it will be available in the library.
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
