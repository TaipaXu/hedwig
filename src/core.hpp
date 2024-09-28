#pragma once

#if defined __linux__ || defined __APPLE__
#include <sys/types.h>
#endif

class Core
{
public:
    constexpr Core() = default;
    ~Core() = default;

    void startAsync() const;
    void stop();

#if defined __linux__ || defined __APPLE__
private:
    mutable pid_t inhibitPid = 0;
#endif
};
