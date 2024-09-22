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
#include "./core.hpp"
#include "./playground.hpp"

int parseDuration(const std::string &duration)
{
    const std::regex pattern(R"((?:(\d+)h)?(?:(\d+)m)?(?:(\d+)s)?)");
    std::smatch matches;
    if (std::regex_match(duration, matches, pattern))
    {
        int totalSeconds = 0;

        if (matches[1].matched)
        {
            totalSeconds += std::stoi(matches[1]) * 60 * 60;
        }
        if (matches[2].matched)
        {
            totalSeconds += std::stoi(matches[2]) * 60;
        }
        if (matches[3].matched)
        {
            totalSeconds += std::stoi(matches[3]);
        }
        return totalSeconds;
    }
    throw std::invalid_argument("Invalid duration format. Supported formats: 30s, 10m, 1h, 1h30m, 1h30m20s, etc.");
}

int main(int argc, char *argv[])
{
    namespace po = boost::program_options;

    po::options_description optionsDescription("Allowed options");
    optionsDescription.add_options()("help,h", "Show help message")                                                         // help option
        ("version,v", "Show version information")                                                                           // version option
        ("command,c", po::value<std::vector<std::string>>(), "Command to execute")                                          // command option
        ("duration,d", po::value<std::string>(), "Duration to prevent screen sleep (e.g., 30s, 10m, 1h, 1h30m, 1h30m20s)"); // duration option

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

    Core core;
    Playground playground;

    std::future<void> coreFuture = core.startAsync();
    std::future<void> playgroundFuture = playground.startAsync();

    if (variablesMap.count("command")) [[likely]]
    {
        const std::vector<std::string> arguments = variablesMap["command"].as<std::vector<std::string>>();
        const std::string command = std::accumulate(arguments.begin(), arguments.end(), std::string(), [](const std::string &a, const std::string &b) { return a + " " + b; });
        const int result = std::system(command.c_str());
        core.stop();
        playground.stop();
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
        core.stop();
        playground.stop();
    }

    coreFuture.wait();
    playgroundFuture.wait();

    std::cout << "Task complete!" << std::endl;

    return EXIT_SUCCESS;
}
