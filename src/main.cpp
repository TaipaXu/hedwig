#include "core.hpp"
#include <iostream>
#include <numeric>
#include <vector>
#include <atomic>
#include <future>
#include <cstdlib>
#include <regex>
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

int parseDuration(const std::string &duration)
{
    const std::regex pattern("(\\d+)([smh])");
    std::smatch matches;
    if (std::regex_match(duration, matches, pattern))
    {
        const int value = std::stoi(matches[1]);
        const char unit = matches[2].str()[0];
        switch (unit)
        {
        case 's':
            return value;
        case 'm':
            return value * 60;
        case 'h':
            return value * 60 * 60;
        default:
            throw std::invalid_argument("Invalid time unit");
        }
    }
    throw std::invalid_argument("Invalid duration format");
}

int main(int argc, char *argv[])
{
    namespace po = boost::program_options;

    po::options_description optionsDescription("Allowed options");
    optionsDescription.add_options()("help,h", "Show help message")                                        // help option
        ("version,v", "Show version information")                                                          // version option
        ("command,c", po::value<std::vector<std::string>>(), "Command to execute")                         // command option
        ("duration,d", po::value<std::string>(), "Duration to prevent screen sleep (e.g., 30s, 10m, 1h)"); // duration option

    po::positional_options_description positionalOptionsDescription;
    positionalOptionsDescription.add("command", -1);

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

    if (variablesMap.count("command")) [[likely]]
    {
        const std::vector<std::string> arguments = variablesMap["command"].as<std::vector<std::string>>();
        const std::string command = std::accumulate(arguments.begin(), arguments.end(), std::string(), [](const std::string &a, const std::string &b) { return a + " " + b; });
        const int result = std::system(command.c_str());
        stopFlag = true;
    }
    else
    {
        if (variablesMap.count("duration"))
        {
            const int durationSeconds = parseDuration(variablesMap["duration"].as<std::string>());
            std::this_thread::sleep_for(std::chrono::seconds(durationSeconds));
        }
        else
        {
            std::cout << "Press Enter to stop..." << std::endl;
            std::cin.get();
        }
        stopFlag = true;
    }

    coreFuture.wait();
    printRunningFuture.wait();

    std::cout << "Task complete!" << std::endl;

    return EXIT_SUCCESS;
}
