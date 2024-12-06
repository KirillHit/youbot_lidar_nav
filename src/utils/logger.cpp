#include <chrono>
#include <format>
#include <string>

#include "youbot_lidar_nav/utils/logger.hpp"
#include "youbot_lidar_nav/utils/parameter_server.hpp"

#define RESET "\033[0m"
#define BLACK "\033[30m"   /* Black */
#define RED "\033[31m"     /* Red */
#define GREEN "\033[32m"   /* Green */
#define YELLOW "\033[33m"  /* Yellow */
#define BLUE "\033[34m"    /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m"    /* Cyan */
#define WHITE "\033[37m"   /* White */

namespace ybotln
{

Logger &Logger::get_logger()
{
    static Logger single_instance;
    return single_instance;
}

Logger::Logger() {}

void Logger::update_parameters()
{
    debug = PARAMETERS.get<bool>("logger/debug");
    save_path = PARAMETERS.get<std::string>("logger/save_path");
    save = PARAMETERS.get<bool>("logger/save");
}

void Logger::operator()(const MSG_LVL &lvl, const std::string &str)
{
    if ((lvl == MSG_LVL::DEBUG) && !debug)
    {
        return;
    }

    constexpr std::string_view msg_lvl[] = {"INFO ", "DEBUG", "WARN ", "ERROR", "FATAL"};
    constexpr std::string_view msg_clr[] = {RESET, CYAN, YELLOW, RED, RED};

    std::string log_str =
        std::format("[{}][{}]: {}", get_time(), msg_lvl[static_cast<size_t>(lvl)], str);

    std::cout << msg_clr[static_cast<size_t>(lvl)] << log_str << RESET << std::endl;

    if (save)
    {
        // TODO
    }
}

std::string Logger::get_time()
{
    using namespace std::chrono;
    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    auto timer = system_clock::to_time_t(now);
    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;
    oss << std::put_time(&bt, "%H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

} // namespace ybotln
