#pragma once

#include <atomic>
#include <future>

class Core
{
public:
    constexpr Core() = default;
    ~Core() = default;

    void start(std::atomic<bool> &stopFlag) const;
    std::future<void> startAsync(std::atomic<bool> &stopFlag) const;

private:
};
