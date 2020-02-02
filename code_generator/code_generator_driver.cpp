#include "code_generator_driver.hpp"

#include "object_file_generator/object_file_generator.hpp"

#include <fstream>

using namespace SLang::CodeGenerator;

using namespace std;

bool Driver::Main(const CommandLineArgs &args) noexcept
{
    const auto &in_path = args.InputPath();
    if (error_code ec; !filesystem::exists(in_path, ec) || ec)
    {
        // TODO: message on file not found.
        return false;
    }

    ifstream in_file(in_path);
    if (!in_file.is_open())
    {
        // TODO: message on file open failure.
        return false;
    }

    // TODO: move to `noexcept` version when it will be available in the library.
    const auto parsed_content = nlohmann::json::parse(in_file, nullptr, false);
    if (parsed_content.is_discarded())
    {
        // TODO: message on parsing failure.
        return false;
    }

    if (!ObjectFileGenerator::ConvertIrToObjectFile(parsed_content, args.OutputPath()))
    {
        // TODO: message on code generation failure.
        return false;
    }

    return true;
}
