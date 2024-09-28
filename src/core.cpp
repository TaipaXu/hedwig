#include "./core.hpp"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void Core::startAsync() const
{
    inhibitPid = fork();
    if (inhibitPid == 0)
    {
#ifdef __linux__
        execlp("systemd-inhibit", "systemd-inhibit", "sleep", "infinity", NULL);
        std::cerr << "Failed to start systemd-inhibit on Linux" << std::endl;
#elif __APPLE__
        execlp("caffeinate", "caffeinate", NULL);
        std::cerr << "Failed to start caffeinate on macOS" << std::endl;
#else
        std::cerr << "Unsupported platform" << std::endl;
#endif
        exit(EXIT_FAILURE);
    }
    else if (inhibitPid < 0)
    {
        std::cerr << "Failed to fork process" << std::endl;
        return;
    }
}

void Core::stop()
{
    if (inhibitPid > 0)
    {
        kill(inhibitPid, SIGTERM);
        waitpid(inhibitPid, NULL, 0);
        inhibitPid = 0;
    }
}
