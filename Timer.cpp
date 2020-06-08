#include "Timer.h"

using namespace std;


float Timer::getSeconds() {
    auto endTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    long long theTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - time_).count();
    float milliseconds = theTime % 1000;
    float seconds = theTime / 1000;
    float decimals = milliseconds / 1000.f;
    return seconds + decimals;
}

void Timer::startClock() {
    time_ = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
}