#ifndef CODE_GENERATOR_DRIVER_HPP
#define CODE_GENERATOR_DRIVER_HPP

#include "command_line_args/command_line_args.hpp"

namespace SLang::CodeGenerator::Driver
{
    /**
     * \brief Execute code generator.
     * \param args Command line arguments.
     * \return `true` - program has finished successfully; `false` - otherwise.
     */
    [[nodiscard]] bool Main(const CommandLineArgs &args) noexcept;
}

#endif
