#include "core.hpp"
#include <cstdlib>

int main(int argc, char *argv[])
{
    constexpr const Core core;
    std::future<void> future = core.startAsync();
    future.wait();

    return EXIT_SUCCESS;
}
