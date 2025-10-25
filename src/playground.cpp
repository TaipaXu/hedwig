#include "./playground.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

constexpr const char *carriageReturn = "\r";

void Playground::start() const
{
    stopFlag.store(false, std::memory_order_relaxed);
    std::vector<std::string> symbols = {"|", "/", "-", "\\"};
    int index = 0;

    while (!stopFlag.load(std::memory_order_relaxed))
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
    stopFlag.store(true, std::memory_order_relaxed);
}
