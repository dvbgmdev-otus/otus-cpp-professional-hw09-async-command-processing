#include "app.h"

#include <cstddef>
#include <ctime>
#include <utility>

#include "bulk_processor.h"
#include "bulk_writer.h"
#include "cli_args.h"

namespace {

constexpr int EXIT_SUCCESS_CODE = 0;
constexpr int EXIT_ERROR_CODE = 1;

std::time_t current_time() { return std::time(nullptr); }

void write_block(ConsoleBulkWriter& console_writer,
                 FileBulkWriter& file_writer,
                 const CommandBlock& block) {
    console_writer.write(block);
    file_writer.write(block);
}

BulkProcessor::CommandBlockHandler make_block_handler(ConsoleBulkWriter& console_writer,
                                                      FileBulkWriter& file_writer) {
    return [&console_writer, &file_writer](const CommandBlock& block) {
        write_block(console_writer, file_writer, block);
    };
}

CommandReader::CommandHandler make_command_handler(BulkProcessor& processor) {
    return [&processor](const std::string& command, std::time_t received_at) {
        processor.process_command(command, received_at);
    };
}

CommandReader::FinishHandler make_finish_handler(BulkProcessor& processor) {
    return [&processor]() { processor.finish(); };
}

void process_input(std::istream& input,
                   std::ostream& output,
                   std::size_t block_size,
                   CommandReader::Clock clock) {
    ConsoleBulkWriter console_writer(output);
    FileBulkWriter file_writer;
    BulkProcessor processor(block_size, make_block_handler(console_writer, file_writer));
    CommandReader reader(
        input, make_command_handler(processor), make_finish_handler(processor), std::move(clock));

    reader.read_all();
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

    process_input(streams.input, streams.output, cli_args.block_size, std::move(clock));

    return EXIT_SUCCESS_CODE;
}
