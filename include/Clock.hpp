#pragma once

#include <chrono>
#include <cstdint>

class Duration
{
public:
    explicit Duration(std::chrono::high_resolution_clock::duration duration);

    [[nodiscard]] double asSeconds() const;
    [[nodiscard]] uint64_t asMilliseconds() const;
    [[nodiscard]] uint64_t asMicroseconds() const;
    [[nodiscard]] uint64_t asNanoseconds() const;

private:
    std::chrono::high_resolution_clock::duration m_duration;
};

class Clock
{
public:
    Clock();
    void restart();

    [[nodiscard]] Duration getElapsedTime() const;

private:
    std::chrono::high_resolution_clock::time_point m_now;
};