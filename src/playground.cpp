#include "./playground.hpp"
#include <iostream>
#include <vector>

constexpr const char *carriageReturn = "\r";

void Playground::start() const
{
    std::vector<std::string> symbols = {"|", "/", "-", "\\"};
    int index = 0;

    while (!stopFlag)
    {
        std::cout << "running " << symbols[index] << carriageReturn << std::flush;
        index = (index + 1) % symbols.size();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

std::future<void> Playground::startAsync() const
{
    return std::async(std::launch::async, [this]() { this->start(); });
}

void Playground::stop()
{
    stopFlag = true;
}
