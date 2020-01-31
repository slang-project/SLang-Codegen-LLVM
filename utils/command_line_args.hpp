#ifndef COMMAND_LINE_ARGS_H
#define COMMAND_LINE_ARGS_H

#include <filesystem>
#include <optional>

namespace CodeGenerator
{
	/**
	 * \brief Command line arguments container of the code generator.
	 */
	class CommandLineArgs
	{
		using PathType = std::filesystem::path;

		PathType in_path_, out_path_;

		CommandLineArgs() noexcept = default;

	public:
		/**
         * \brief Parse provided command line arguments for code generator.
         * \param argv Pointer to array of c-style strings of arguments (like `main`'s `argv`).
         * \param argc Size of the array `argv` (like `main`'s argc).
         * \return `std::nullopt` - parsing failed; parsed command line arguments' values - otherwise.
         * \pre `argv` is pointer to continuously allocated memory of length `argc`.
         * \pre Each element pointed by elements of `argv` is null-terminated c-style string.
         * \note The order of parameters was changed due to possible usage of `std::span`.
         */
		[[nodiscard]] static std::optional<CommandLineArgs> Parse(char const * const * argv, int argc) noexcept;

		/**
		 * \brief Get input file path.
		 * \return Input file path object.
		 */
		[[nodiscard]] const PathType &InputPath() const
		{
			return in_path_;
		}

		/**
		 * \brief Get output file path.
		 * \return Output file path object.
		 */
		[[nodiscard]] const PathType &OutputPath() const
		{
			return out_path_;
		}
	};
}

#endif
