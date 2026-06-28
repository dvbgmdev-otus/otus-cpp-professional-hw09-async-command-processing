#include <iostream>

#include "app.h"

int main(int argc, const char* const argv[]) {
    return run_app(argc, argv, AppStreams{ std::cin, std::cout, std::cerr });
}
