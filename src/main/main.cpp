#include <cstddef>
#include <iostream>

#include "async.h"
#include "cli_args.h"

namespace {

constexpr int EXIT_SUCCESS_CODE = 0;
constexpr int EXIT_ERROR_CODE = 1;

void process_input(std::size_t block_size) {
    constexpr std::size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    async::handle_t handle = async::connect(block_size);
    while (std::cin) {
        std::cin.read(buffer, BUFFER_SIZE);
        const std::streamsize received = std::cin.gcount();
        if (received > 0) {
            async::receive(handle, buffer, static_cast<std::size_t>(received));
        }
    }
    async::disconnect(handle);
}

}  // namespace

int main(int argc, const char* const argv[]) {
    const CliArgsParseResult cli_args = parse_cli_args(argc, argv);
    if (!cli_args.success) {
        std::cerr << cli_args.error << '\n';
        return EXIT_ERROR_CODE;
    }

    process_input(cli_args.block_size);

    return EXIT_SUCCESS_CODE;
}
