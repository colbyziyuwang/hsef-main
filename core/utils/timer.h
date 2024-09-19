#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>
#include <ctime>

/**
 * Defines a class for a timer object.
 *
 * TODO Add storing of time points.
 * @class Timer
 */
class Timer {
public:
    /**
     * Constructs a new timer object.
     */
    Timer() = default;

    /**
     * Starts the timer. Restarts if it has already happened.
     */
    void startTimer();

    /**
     * Ends the timer.
     */
    void endTimer();

    /**
     * Returns how much time has elapsed since the last time the timer was started.
     *
     * @todo Handle error that occurs when function is called when the timer is not currently timing
     *
     * @return The time elapsed since the timer was started.
     */
    double getCurrentTimePeriodDuration() const;

    /**
     * Returns the duration of the last timed period. This is the period between the last call of startTimer and
     * endTimer. Returns -1 if neither of these functions has been called now.
     *
     * @return The duration of the last timed period.
     */
    double getLastTimePeriodDuration() const;

    /**
     * Returns if the timer is currently being used.
     *
     * @return If the timer is currently timing.
     */
    bool isTiming() const;

private:
    bool m_timing = false;  ///< Whether the timer is currently timing or not.
    double m_last_time = -1.0;  ///< The duration of the last timed period.

    std::chrono::time_point<std::chrono::system_clock> m_start_time;  ///< The last starting time point.
};

#endif /* TIMER_H_ */
