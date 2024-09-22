#include "./core.hpp"
#include <iostream>
#include <sys/wait.h>

void Core::startAsync() const
{
    inhibitPid = fork();
    if (inhibitPid == 0)
    {
        execlp("systemd-inhibit", "systemd-inhibit", "--what=idle", "--who=my_program", "--why=Prevent screen locking", "sleep", "infinity", NULL);
        std::cerr << "Failed to start systemd-inhibit" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (inhibitPid < 0)
    {
        std::cerr << "Failed to fork process for systemd-inhibit" << std::endl;
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
