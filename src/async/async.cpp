#include "async.h"

#include <ctime>

#include "async_context.h"
#include "async_runtime.h"

namespace {

std::time_t current_time() { return std::time(nullptr); }

AsyncContext* to_context(async::handle_t handle) {
    return static_cast<AsyncContext*>(handle);
}

}  // namespace

namespace async {

handle_t connect(std::size_t bulk) {
    return new AsyncContext(
        bulk, [](const CommandBlock& block) { AsyncRuntime::instance().publish(block); },
        current_time);
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

    AsyncRuntime::instance().wait();
}

}  // namespace async
