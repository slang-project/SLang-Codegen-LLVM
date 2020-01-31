#ifndef CODE_GENERATOR_DRIVER_H
#define CODE_GENERATOR_DRIVER_H

#include "utils/command_line_args.hpp"

namespace CodeGenerator::Driver
{
	/**
	 * \brief Execute code generator.
	 * \param args Command line arguments.
	 * \return `true` - program has finished successfully; `false` - otherwise.
	 */
	[[nodiscard]] bool Main(const CommandLineArgs &args) noexcept;
}

#endif
