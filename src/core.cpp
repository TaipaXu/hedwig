#include "./core.hpp"
#include <iostream>
#include <thread>
#include <chrono>
extern "C"
{
#include <X11/Xlib.h>
}

void Core::start() const
{
    Display *display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        std::cerr << "Cannot open display" << std::endl;
        return;
    }

    const Window root = XDefaultRootWindow(display);

    int originalX;
    int originalY;
    Window window;
    int winX;
    int winY;
    unsigned int mask_return;
    XQueryPointer(display, root, &window, &window, &originalX, &originalY, &winX, &winY, &mask_return);

    constexpr const int offsetX = 2;
    constexpr const int offsetY = 2;

    while (true)
    {
        XWarpPointer(display, None, root, 0, 0, 0, 0, originalX + offsetX, originalY + offsetY);
        XFlush(display);

        constexpr const std::chrono::milliseconds recoveryDelay = std::chrono::milliseconds(5);
        std::this_thread::sleep_for(recoveryDelay);

        XWarpPointer(display, None, root, 0, 0, 0, 0, originalX, originalY);
        XFlush(display);

        constexpr const std::chrono::seconds delay = std::chrono::seconds(10);
        std::this_thread::sleep_for(delay);
    }

    XCloseDisplay(display);
}

std::future<void> Core::startAsync() const
{
    return std::async(std::launch::async, [this]()
                      { this->start(); });
}
