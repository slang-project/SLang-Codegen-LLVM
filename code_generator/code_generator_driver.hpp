#ifndef CODE_GENERATOR_DRIVER_H
#define CODE_GENERATOR_DRIVER_H

#include "utils/command_line_args.hpp"

namespace CodeGenerator
{

namespace Driver
{
	/**
	 * \brief Execute code generator.
	 * \param args Command line arguments.
	 * \return `true` - program has finished successfully; `false` - otherwise.
	 */
	bool Main(CommandLineArgs::Arguments args) noexcept;
}

}

#endif
