#pragma once

#include <atomic>
#include <future>

class Playground
{
public:
    Playground() = default;
    ~Playground() = default;

    void start() const;
    std::future<void> startAsync() const;
    void stop();

private:
    mutable std::atomic<bool> stopFlag = false;
};
