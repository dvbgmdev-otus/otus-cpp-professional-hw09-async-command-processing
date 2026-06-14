#include "cli_args.h"

#include <cctype>
#include <cerrno>
#include <cstdlib>

namespace {

CliArgsParseResult make_error(const std::string& error) {
    return CliArgsParseResult{ false, 0, error };
}

CliArgsParseResult make_success(std::size_t block_size) {
    return CliArgsParseResult{ true, block_size, {} };
}

bool is_decimal_number(const char* value) {
    const char* current = value;
    while (*current != '\0') {
        if (!std::isdigit(static_cast<unsigned char>(*current))) {
            return false;
        }
        ++current;
    }
    return current != value;
}

}  // namespace

CliArgsParseResult parse_cli_args(int argc, const char* const argv[]) {
    if (argc != 2) {
        return make_error("Expected one argument: block size");
    }

    const char* value = argv[1];
    if (!is_decimal_number(value)) {
        return make_error("Block size must be a positive integer");
    }

    errno = 0;
    const unsigned long long parsed = std::strtoull(value, nullptr, 10);
    if (errno == ERANGE || parsed == 0) {
        return make_error("Block size must be a positive integer");
    }

    return make_success(static_cast<std::size_t>(parsed));
}
