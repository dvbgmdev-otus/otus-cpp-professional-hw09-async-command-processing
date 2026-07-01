#include "app.h"

#include <ctime>
#include <cstddef>
#include <istream>
#include <utility>

#include "async.h"
#include "async_internal.h"
#include "cli_args.h"

namespace {

constexpr int EXIT_SUCCESS_CODE = 0;
constexpr int EXIT_ERROR_CODE = 1;

std::time_t current_time() { return std::time(nullptr); }

void process_input(std::istream& input, std::size_t block_size) {
    constexpr std::size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    async::handle_t handle = async::connect(block_size);
    while (input) {
        input.read(buffer, BUFFER_SIZE);
        const std::streamsize received = input.gcount();
        if (received > 0) {
            async::receive(handle, buffer, static_cast<std::size_t>(received));
        }
    }
    async::disconnect(handle);
}

}  // namespace

int run_app(int argc, const char* const argv[], AppStreams streams, CommandReader::Clock clock) {
    if (!clock) {
        clock = current_time;
    }

    const CliArgsParseResult cli_args = parse_cli_args(argc, argv);
    if (!cli_args.success) {
        streams.error << cli_args.error << '\n';
        return EXIT_ERROR_CODE;
    }

    async::detail::ScopedSettings async_settings(streams.output, std::move(clock));
    process_input(streams.input, cli_args.block_size);

    return EXIT_SUCCESS_CODE;
}
