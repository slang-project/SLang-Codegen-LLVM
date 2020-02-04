#ifndef CODE_GENERATOR_DRIVER_HPP
#define CODE_GENERATOR_DRIVER_HPP

#include "command_line_args/command_line_args.hpp"

#include <iostream>

namespace SLang::CodeGenerator::Driver
{
    /**
     * \brief Execute code generator.
     * \param args Command line arguments.
     * \param outs Stream where program's wide character output will go.
     * \param errs Stream where program's wide character error will go.
     * \param logs Stream where program's wide character log will go.
     * \return `true` - program has finished successfully; `false` - otherwise.
     */
    [[nodiscard]] bool Main(
        const CommandLineArgs &args,
        std::wostream &outs = std::wcout,
        std::wostream &errs = std::wcerr,
        std::wostream &logs = std::wclog) noexcept;
}

#endif
