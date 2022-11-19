#include "../include/Clock.hpp"
#include <ratio>

Duration::Duration(const std::chrono::high_resolution_clock::duration duration) : m_duration(duration)
{
}

double Duration::asSeconds() const noexcept
{
    return std::chrono::duration_cast<std::chrono::duration<double>>(m_duration).count();
}
uint64_t Duration::asMilliseconds() const noexcept
{
    return std::chrono::duration_cast<std::chrono::duration<uint64_t, std::milli>>(m_duration).count();
}
uint64_t Duration::asMicroseconds() const noexcept
{
    return std::chrono::duration_cast<std::chrono::duration<uint64_t, std::micro>>(m_duration).count();
}
uint64_t Duration::asNanoseconds() const noexcept
{
    return std::chrono::duration_cast<std::chrono::duration<uint64_t, std::nano>>(m_duration).count();
}

Clock::Clock()
{
    restart();
}

void Clock::restart() noexcept
{
    m_now = std::chrono::high_resolution_clock::now();
}

Duration Clock::getElapsedTime() const noexcept
{
    return Duration{std::chrono::high_resolution_clock::now() - m_now};
}