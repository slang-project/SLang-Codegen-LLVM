#ifndef COMMAND_LINE_ARGS_H
#define COMMAND_LINE_ARGS_H

#include <optional>

namespace CodeGenerator
{

namespace CommandLineArgs
{
	/**
	 * \brief Command line arguments container of the code generator.
	 */
	class Arguments
	{};

	/**
	 * \brief Parse provided command line arguments for code generator.
	 * \param argv Pointer to array of c-style strings of arguments (like `main`'s `argv`).
	 * \param argc Size of the array `argv` (like `main`'s argc).
	 * \return `std::nullopt` - parsing failed; parsed command line arguments' values - otherwise.
	 * \pre `argv` is pointer to continuously allocated memory of length `argc`.
	 * \pre Each element pointed by elements of `argv` is null-terminated c-style string.
	 * \note The order of parameters was changed due to possible usage of `std::span`.
	 */
	std::optional<Arguments> Parse(char const * const * argv, int argc) noexcept;
}

}

#endif
