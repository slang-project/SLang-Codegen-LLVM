#include "command_line_args.hpp"

using namespace SLang::CodeGenerator;

using namespace std;

optional<CommandLineArgs> CommandLineArgs::Parse(
    const vector<string_view> &args,
    wostream &outs,
    wostream &errs,
    wostream &logs) noexcept
{
    CommandLineArgs result;

    if (args.size() < 2)
    {
        return nullopt;
    }

    // TODO: add support for help info flag.

    // TODO: use constructor of `std::filesystem::path` with C++20 (`u8path` will be deprecated).
    result.in_path_ = filesystem::u8path(args[1]).lexically_normal();

    (result.out_path_ = result.InputPath()).replace_extension(L".obj");

    return result;
}

auto CommandLineArgs::InputPath() const noexcept -> const PathType&
{
    return in_path_;
}

auto CommandLineArgs::OutputPath() const noexcept -> const PathType&
{
    return out_path_;
}
