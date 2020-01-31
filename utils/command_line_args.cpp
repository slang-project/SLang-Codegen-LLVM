#include "command_line_args.hpp"

#include <cassert>
#include <cstddef>

using namespace CodeGenerator;
using namespace std;

optional<CommandLineArgs> CommandLineArgs::Parse(char const * const * const argv, int const argc) noexcept
{
	assert(argv[argc] == NULL);

	CommandLineArgs result;

	if (argc < 2)
	{
		return nullopt;
	}

	result.in_path_ = std::filesystem::u8path(argv[1]);

	(result.out_path_ = result.InputPath()).replace_extension(u8".obj");

	return result;
}
