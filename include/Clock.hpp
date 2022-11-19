#pragma once

#include <chrono>
#include <cstdint>

class Duration
{
public:
    explicit Duration(std::chrono::high_resolution_clock::duration duration);

    [[nodiscard]] double asSeconds() const noexcept;
    [[nodiscard]] uint64_t asMilliseconds() const noexcept;
    [[nodiscard]] uint64_t asMicroseconds() const noexcept;
    [[nodiscard]] uint64_t asNanoseconds() const noexcept;

private:
    std::chrono::high_resolution_clock::duration m_duration;
};

class Clock
{
public:
    Clock();

    void restart() noexcept;

    [[nodiscard]] Duration getElapsedTime() const noexcept;

private:
    std::chrono::high_resolution_clock::time_point m_now;
};