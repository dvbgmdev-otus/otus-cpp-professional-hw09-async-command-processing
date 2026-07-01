#include "async_context.h"

#include <ctime>
#include <utility>

namespace {

std::time_t current_time() { return std::time(nullptr); }

}  // namespace

AsyncContext::AsyncContext(std::size_t block_size, CommandBlockHandler command_block_handler)
    : m_processor(block_size, std::move(command_block_handler)),
      m_pending_line() {}

void AsyncContext::receive(const char* data, std::size_t size) {
    if (data == nullptr || size == 0) {
        return;
    }

    for (std::size_t i = 0; i < size; ++i) {
        if (data[i] == '\n') {
            process_line(m_pending_line);
            m_pending_line.clear();
            continue;
        }

        m_pending_line.push_back(data[i]);
    }
}

void AsyncContext::disconnect() {
    if (!m_pending_line.empty()) {
        process_line(m_pending_line);
        m_pending_line.clear();
    }

    m_processor.finish();
}

void AsyncContext::process_line(const std::string& line) {
    m_processor.process_command(line, current_time());
}
