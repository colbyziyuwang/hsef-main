#include "timer.h"
#include <chrono>

void Timer::startTimer() {
    m_start_time = std::chrono::system_clock::now();
    m_timing = true;
}

void Timer::endTimer() {
    if (!m_timing) {
        return;
    }
    std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - m_start_time;

    m_last_time = elapsed.count();
    m_timing = false;
}

double Timer::getCurrentTimePeriodDuration() const {
    //TODO Handle the error when the timer is not currently timing
    if (!m_timing) {
        return -1.0;
    }

    std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - m_start_time;

    return elapsed.count();
}

double Timer::getLastTimePeriodDuration() const {
    return m_last_time;
}

bool Timer::isTiming() const {
    return m_timing;
}
