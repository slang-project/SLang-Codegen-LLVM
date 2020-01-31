#include "code_generator_driver.hpp"

#include <cstdlib>

using namespace CodeGenerator;

int main(int const argc, char const * const * const argv) noexcept
{
	if (const auto args = CommandLineArgs::Parse(argv, argc); args.has_value())
	{
		return Driver::Main(*args) ? EXIT_SUCCESS : EXIT_FAILURE;
	}

	return EXIT_FAILURE;
}
