#pragma once

#include <atomic>
#include <future>

class Core
{
public:
    constexpr Core() = default;
    ~Core() = default;

    void startAsync() const;
    void stop();

private:
    mutable pid_t inhibitPid = 0;
};
