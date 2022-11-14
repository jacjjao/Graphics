#include "../include/Clock.hpp"

Time::Time(const std::chrono::high_resolution_clock::duration duration) : m_duration(duration)
{
}

double Time::as_seconds() const
{
    return std::chrono::duration_cast<std::chrono::duration<double>>(m_duration).count();
}
uint64_t Time::as_milliseconds() const
{
    return std::chrono::duration_cast<std::chrono::duration<uint64_t, std::milli>>(m_duration).count();
}
uint64_t Time::as_microseconds() const
{
    return std::chrono::duration_cast<std::chrono::duration<uint64_t, std::micro>>(m_duration).count();
}
uint64_t Time::as_nanoseconds() const
{
    return std::chrono::duration_cast<std::chrono::duration<uint64_t, std::nano>>(m_duration).count();
}

Clock::Clock()
{
    restart();
}

Time Clock::get_elapsed_time() const
{
    return Time{std::chrono::high_resolution_clock::now() - m_now};
}

void Clock::restart()
{
    m_now = std::chrono::high_resolution_clock::now();
}