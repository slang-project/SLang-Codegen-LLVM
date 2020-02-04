#include "code_generator_driver.hpp"

#include <cstdlib>
#include <string_view>
#include <vector>

using namespace SLang::CodeGenerator;

using namespace std;

/**
 * \brief Parse provided command line arguments for code generator.
 * \param argv Pointer to array of c-style strings of arguments (like `main`'s `argv`).
 * \param argc Size of the array `argv` (like `main`'s argc).
 * \return `std::vector` of `std::string_view` pointing to values pointed by `argv`'s.
 * \pre `argv` is pointer to contiguous memory block containing `argc` number of elements.
 * \pre Each element pointed by elements of `argv` is null-terminated c-style string.
 * \note The order of parameters was changed due to possible usage of `std::span`.
 */
static auto ArgsToVector(const char * const * const argv, const int argc) noexcept
// TODO: use `const std::span<const char *> &` instead of `argv` and `argc` in C++20.
{
    vector<string_view> result;
    result.reserve(argc);

    // TODO: use range-based for loop
    for (auto i = static_cast<decltype(argc)>(0); i < argc; ++i)
    {
        result.emplace_back(argv[i]);
    }

    return result;
}

int main(const int argc, const char * const * const argv) noexcept
{
    ios_base::sync_with_stdio(false);

    if (const auto args = CommandLineArgs::Parse(ArgsToVector(argv, argc)); args.has_value())
    {
        return Driver::Main(*args) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}
