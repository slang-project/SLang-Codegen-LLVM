#include "code_generator_driver.hpp"

#include <cstdlib>

using namespace CodeGenerator;

int main(int const argc, char const * const * const argv) noexcept
try
{
	return Driver::Main(CommandLineArgs::Parse(argv, argc).value())
		? EXIT_SUCCESS
		: EXIT_FAILURE;
}
catch (const std::bad_optional_access &)
{
	return EXIT_FAILURE;
}
