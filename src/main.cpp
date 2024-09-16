#include "core.hpp"
#include <cstdlib>

int main(int argc, char *argv[])
{
    constexpr const Core core;
    core.start();

    return EXIT_SUCCESS;
}
