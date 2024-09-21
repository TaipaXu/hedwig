#pragma once

#include <atomic>
#include <future>

class Core
{
public:
    constexpr Core() = default;
    ~Core() = default;

    void start() const;
    std::future<void> startAsync() const;
    void stop();

private:
    mutable std::atomic<bool> stopFlag = false;
};
