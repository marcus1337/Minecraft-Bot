#pragma once
#include <chrono>

class Timer {
    template <class Duration>
    using sys_time = std::chrono::time_point<std::chrono::system_clock, Duration>;
    sys_time<std::chrono::milliseconds> time_;

public:

    float getSeconds();
    void startClock();
};