#pragma once

#include <future>

class Core
{
public:
    constexpr Core() = default;
    ~Core() = default;

    void start() const;
    std::future<void> startAsync() const;

private:
};
