/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, May, 2025                                                     *
 * Title           - mirror_raytracer [WSL: Debian]                                     *
 * Description     -                                                                    *
 *     Timer                                                                            *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Timer.hpp"
#include <iomanip>
#include <sstream>

namespace Raytracer {

Timer::Timer(const std::string& name)
    : name(name)
    , running(false)
{
}

void Timer::start()
{
    if (!running) {
        startTime = std::chrono::high_resolution_clock::now();
        running = true;
    }
}

void Timer::stop()
{
    running = false;
}

void Timer::reset()
{
    startTime = std::chrono::high_resolution_clock::now();
}

double Timer::elapsedSeconds() const
{
    if (!running) {
        return 0.0;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    return duration.count() / 1000000.0;
}

std::string Timer::elapsedString() const
{
    double seconds = elapsedSeconds();
    std::ostringstream oss;

    if (seconds >= 60.0) {
        int minutes = static_cast<int>(seconds / 60.0);
        seconds -= minutes * 60.0;
        oss << minutes << "m ";
    }

    oss << std::fixed << std::setprecision(2) << seconds << "s";
    return oss.str();
}

} // namespace Raytracer