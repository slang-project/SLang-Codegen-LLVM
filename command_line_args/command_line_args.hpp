#ifndef COMMAND_LINE_ARGS_HPP
#define COMMAND_LINE_ARGS_HPP

#include <filesystem>
#include <optional>

namespace SLang::CodeGenerator
{
    /**
     * \brief Command line arguments container of the code generator.
     */
    class CommandLineArgs
    {
    public:
        using PathType = std::filesystem::path;

        /**
         * \brief Parse provided command line arguments for code generator.
         * \param args Collection of `std::string_view`'s on command line argument strings.
         * \return `std::nullopt` - parsing failed; parsed command line arguments' values - otherwise.
         */
        [[nodiscard]] static std::optional<CommandLineArgs> Parse(const std::vector<std::string_view> &args) noexcept;

        /**
         * \brief Get input file path.
         * \return Input file path object.
         */
        [[nodiscard]] const PathType &InputPath() const noexcept;

        /**
         * \brief Get output file path.
         * \return Output file path object.
         */
        [[nodiscard]] const PathType &OutputPath() const noexcept;

    private:
        CommandLineArgs() noexcept = default;

        PathType in_path_, out_path_;
    };
}

#endif
