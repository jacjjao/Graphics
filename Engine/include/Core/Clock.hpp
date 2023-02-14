#pragma once

#include <chrono>
#include <cstdint>

namespace eg
{

    class Duration
    {
    public:
        explicit Duration(std::chrono::high_resolution_clock::duration duration) : m_duration(duration) {}

        [[nodiscard]] 
        double asSeconds() const
        { return std::chrono::duration_cast<std::chrono::duration<double>>(m_duration).count(); }

        [[nodiscard]] 
        uint64_t asMilliseconds() const
        { return std::chrono::duration_cast<std::chrono::duration<uint64_t, std::milli>>(m_duration).count(); }

        [[nodiscard]] 
        uint64_t asMicroseconds() const
        { return std::chrono::duration_cast<std::chrono::duration<uint64_t, std::micro>>(m_duration).count(); }

        [[nodiscard]] 
        uint64_t asNanoseconds() const
        { return std::chrono::duration_cast<std::chrono::duration<uint64_t, std::micro>>(m_duration).count(); }

    private:
        std::chrono::high_resolution_clock::duration m_duration;
    };

    class Clock
    {
    public:
        Clock() { restart(); }

        void restart()
        { m_now = std::chrono::high_resolution_clock::now(); }

        [[nodiscard]] 
        Duration getElapsedTime() const
        { return Duration{ std::chrono::high_resolution_clock::now() - m_now }; }

    private:
        std::chrono::high_resolution_clock::time_point m_now;
    };

} // namespace eg