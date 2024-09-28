#include "./core.hpp"
#include <iostream>
#if defined __linux__ || defined __APPLE__
#include <unistd.h>
#include <sys/wait.h>
#endif
#ifdef _WIN32
#include <windows.h>
#include <thread>
#endif
#include <signal.h>

void Core::startAsync() const
{
#if defined __linux__
    inhibitPid = fork();
    if (inhibitPid == 0)
    {
        execlp("systemd-inhibit", "systemd-inhibit", "sleep", "infinity", NULL);
        std::cerr << "Failed to start systemd-inhibit on Linux" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (inhibitPid < 0)
    {
        std::cerr << "Failed to fork process" << std::endl;
        return;
    }
#elif defined __APPLE__
    inhibitPid = fork();
    if (inhibitPid == 0)
    {
        execlp("caffeinate", "caffeinate", NULL);
        std::cerr << "Failed to start caffeinate on macOS" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (inhibitPid < 0)
    {
        std::cerr << "Failed to fork process" << std::endl;
        return;
    }
#elif defined _WIN32
    std::thread inhibitThread([]() {
        if (SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED) == NULL)
        {
            std::cerr << "Failed to start SetThreadExecutionState on Windows" << std::endl;
        }
        else
        {
            std::cout << "System is now preventing sleep on Windows." << std::endl;
            std::this_thread::sleep_for(std::chrono::hours(24));
        }
    });
    inhibitThread.detach();
#endif
}

void Core::stop()
{
#if defined __linux__ || defined __APPLE__
    if (inhibitPid > 0)
    {
        kill(inhibitPid, SIGTERM);
        waitpid(inhibitPid, NULL, 0);
        inhibitPid = 0;
    }
#elif defined _WIN32
    SetThreadExecutionState(ES_CONTINUOUS);
#endif
}
