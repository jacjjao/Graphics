#include "../include/Clock.hpp"

Duration::Duration(const std::chrono::high_resolution_clock::duration duration) : m_duration(duration)
{
}

double Duration::asSeconds() const
{
    return std::chrono::duration_cast<std::chrono::duration<double>>(m_duration).count();
}
uint64_t Duration::asMilliseconds() const
{
    return std::chrono::duration_cast<std::chrono::duration<uint64_t, std::milli>>(m_duration).count();
}
uint64_t Duration::asMicroseconds() const
{
    return std::chrono::duration_cast<std::chrono::duration<uint64_t, std::micro>>(m_duration).count();
}
uint64_t Duration::asNanoseconds() const
{
    return std::chrono::duration_cast<std::chrono::duration<uint64_t, std::nano>>(m_duration).count();
}

Clock::Clock()
{
    restart();
}

Duration Clock::getElapsedTime() const
{
    return Duration{std::chrono::high_resolution_clock::now() - m_now};
}

void Clock::restart()
{
    m_now = std::chrono::high_resolution_clock::now();
}