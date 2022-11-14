#pragma once

#include <chrono>
#include <ratio>
#include <cstdint>

class Time
{
public:
    explicit Time(std::chrono::high_resolution_clock::duration duration);

    [[nodiscard]] double as_seconds() const;
    [[nodiscard]] uint64_t as_milliseconds() const;
    [[nodiscard]] uint64_t as_microseconds() const;
    [[nodiscard]] uint64_t as_nanoseconds() const;

private:
    std::chrono::high_resolution_clock::duration m_duration;
};

class Clock
{
public:
    Clock();
    void restart();

    [[nodiscard]] Time get_elapsed_time() const;

private:
    std::chrono::high_resolution_clock::time_point m_now;
};