#include "async_runtime.h"

#include <iostream>

#include "bulk_writer.h"

AsyncRuntime& AsyncRuntime::instance() {
    static AsyncRuntime runtime;
    return runtime;
}

AsyncRuntime::AsyncRuntime() : m_output(&std::cout) {}

void AsyncRuntime::publish(const CommandBlock& block) {
    ConsoleBulkWriter console_writer(*m_output);
    FileBulkWriter file_writer;

    console_writer.write(block);
    file_writer.write(block);
}

void AsyncRuntime::set_output(std::ostream& output) {
    m_output = &output;
}

std::ostream& AsyncRuntime::output() const {
    return *m_output;
}
