#include "core.hpp"
#include <iostream>
#include <vector>
#include <atomic>
#include <future>
#include <cstdlib>

constexpr const char *carriageReturn = "\r";

std::future<void> printRunningAsync(std::atomic<bool> &stopFlag)
{
    return std::async(std::launch::async, [&stopFlag]() {
        std::vector<std::string> symbols = {"|", "/", "-", "\\"};
        int index = 0;

        while (!stopFlag)
        {
            std::cout << "running " << symbols[index] << carriageReturn << std::flush;
            index = (index + 1) % symbols.size();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    });
}

int main(int argc, char *argv[])
{
    constexpr const Core core;

    std::atomic<bool> stopFlag(false);
    std::future<void> coreFuture = core.startAsync(stopFlag);
    std::future<void> printRunningFuture = printRunningAsync(stopFlag);

    std::cout << "Press Enter to stop..." << std::endl;
    std::cin.get();
    stopFlag = true;

    coreFuture.wait();
    printRunningFuture.wait();

    std::cout << "Task complete!" << std::endl;

    return EXIT_SUCCESS;
}
