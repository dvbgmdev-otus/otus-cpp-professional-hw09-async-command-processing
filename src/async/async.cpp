#include "async.h"

#include <ctime>
#include <utility>

#include "async_context.h"
#include "async_internal.h"
#include "async_runtime.h"

namespace {

std::time_t current_time() { return std::time(nullptr); }

async::detail::Clock g_clock = current_time;

async::detail::Clock active_clock() { return g_clock; }

AsyncContext* to_context(async::handle_t handle) {
    return static_cast<AsyncContext*>(handle);
}

}  // namespace

namespace async {
namespace detail {

ScopedSettings::ScopedSettings(std::ostream& output, Clock clock)
    : m_previous_output(AsyncRuntime::instance().output()),
      m_previous_clock(std::move(g_clock)) {
    AsyncRuntime::instance().set_output(output);
    g_clock = std::move(clock);
    if (!g_clock) {
        g_clock = current_time;
    }
}

ScopedSettings::~ScopedSettings() {
    AsyncRuntime::instance().set_output(m_previous_output);
    g_clock = std::move(m_previous_clock);
}

}  // namespace detail

handle_t connect(std::size_t bulk) {
    return new AsyncContext(
        bulk, [](const CommandBlock& block) { AsyncRuntime::instance().publish(block); },
        active_clock());
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
