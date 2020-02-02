#ifndef TREE_TRAVERSE_HPP
#define TREE_TRAVERSE_HPP

#define JSON_NOEXCEPTION
#include "nlohmann/json.hpp"
#undef JSON_NOEXCEPTION

#include <filesystem>

namespace SLang::CodeGenerator::ObjectFileGenerator
{
    using SLangIr = nlohmann::json;

    /**
     * \brief Parse given program tree and compile an object file out of it.
     * \param program SLang IR to use for code generation.
     * \param out_path Path to the place where an object file should be stored.
     * \return `true` - file was generated successfully; `false` - otherwise.
     * \note If some file exists at `out_path`, it will be overwritten.
     */
    [[nodiscard]] bool ConvertIrToObjectFile(const SLangIr &program, const std::filesystem::path &out_path) noexcept;
}

#endif
