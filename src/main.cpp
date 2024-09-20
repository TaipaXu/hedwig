#include "core.hpp"
#include <iostream>
#include <vector>
#include <atomic>
#include <future>
#include <cstdlib>
#include <boost/program_options.hpp>
#include "./config.hpp"

constexpr const char *carriageReturn = "\r";

std::future<void> printRunningAsync(std::atomic<bool> &stopFlag)
{
    return std::async(std::launch::async, [&stopFlag]() {
        std::vector<std::string> symbols = {"|", "/", "-", "\\"};
        int index = 0;

        while (!stopFlag)
        {
            std::cout << "running " << symbols[index] << carriageReturn << std::flush;
            index = (index + 1) % symbols.size();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    });
}

int main(int argc, char *argv[])
{
    namespace po = boost::program_options;

    po::options_description optionsDescription("Allowed options");
    optionsDescription.add_options()("help,h", "Show help message") // help option
        ("version,v", "Show version information");                  // version option

    po::positional_options_description positionalOptionsDescription;

    po::variables_map variablesMap;
    try
    {
        po::store(po::command_line_parser(argc, argv)
                      .options(optionsDescription)
                      .positional(positionalOptionsDescription)
                      .run(),
                  variablesMap);
        po::notify(variablesMap);
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    if (variablesMap.count("help"))
    {
        std::cout << optionsDescription << std::endl;
        return EXIT_SUCCESS;
    }
    else if (variablesMap.count("version"))
    {
        std::cout << DISPLAY_NAME << " " << PROJECT_VERSION << std::endl;
        return EXIT_SUCCESS;
    }
    constexpr const Core core;

    std::atomic<bool> stopFlag(false);
    std::future<void> coreFuture = core.startAsync(stopFlag);
    std::future<void> printRunningFuture = printRunningAsync(stopFlag);

    std::cout << "Press Enter to stop..." << std::endl;
    std::cin.get();
    stopFlag = true;

    coreFuture.wait();
    printRunningFuture.wait();

    std::cout << "Task complete!" << std::endl;

    return EXIT_SUCCESS;
}
