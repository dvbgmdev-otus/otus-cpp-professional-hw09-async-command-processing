#include "async.h"

#include <ctime>
#include <iostream>
#include <utility>

#include "async_context.h"
#include "async_internal.h"
#include "bulk_writer.h"

namespace {

std::time_t current_time() { return std::time(nullptr); }

std::ostream* g_output = &std::cout;
async::detail::Clock g_clock = current_time;

std::ostream& active_output() { return *g_output; }

async::detail::Clock active_clock() { return g_clock; }

void write_block(const CommandBlock& block) {
    ConsoleBulkWriter console_writer(active_output());
    FileBulkWriter file_writer;

    console_writer.write(block);
    file_writer.write(block);
}

AsyncContext* to_context(async::handle_t handle) {
    return static_cast<AsyncContext*>(handle);
}

}  // namespace

namespace async {
namespace detail {

ScopedSettings::ScopedSettings(std::ostream& output, Clock clock)
    : m_previous_output(g_output),
      m_previous_clock(std::move(g_clock)) {
    g_output = &output;
    g_clock = std::move(clock);
    if (!g_clock) {
        g_clock = current_time;
    }
}

ScopedSettings::~ScopedSettings() {
    g_output = m_previous_output;
    g_clock = std::move(m_previous_clock);
}

}  // namespace detail

handle_t connect(std::size_t bulk) {
    return new AsyncContext(
        bulk, [](const CommandBlock& block) { write_block(block); }, active_clock());
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
