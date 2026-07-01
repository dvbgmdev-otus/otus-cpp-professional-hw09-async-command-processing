#include "async.h"

#include <iostream>

#include "async_context.h"
#include "bulk_writer.h"

namespace {

void write_block(const CommandBlock& block) {
    ConsoleBulkWriter console_writer(std::cout);
    FileBulkWriter file_writer;

    console_writer.write(block);
    file_writer.write(block);
}

AsyncContext* to_context(async::handle_t handle) {
    return static_cast<AsyncContext*>(handle);
}

}  // namespace

namespace async {

handle_t connect(std::size_t bulk) {
    return new AsyncContext(bulk, [](const CommandBlock& block) { write_block(block); });
}

void receive(handle_t handle, const char* data, std::size_t size) {
    AsyncContext* context = to_context(handle);
    if (context == nullptr) {
        return;
    }

    context->receive(data, size);
}

void disconnect(handle_t handle) {
    AsyncContext* context = to_context(handle);
    if (context == nullptr) {
        return;
    }

    context->disconnect();
    delete context;
}

}  // namespace async
