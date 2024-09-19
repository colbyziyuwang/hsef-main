#include <chrono>
#include <gtest/gtest.h>
#include <thread>

#include "utils/timer.h"

/**
 * Creates a Timer object and tests whether its variables concur with its expected status.
 * Initially tests the variables before starting the timer. Then, tests variables and 
 * expected time values after we start the timer. We expect calls to 
 * getCurrentTimePeriodDuration() return values not less than the previous calls to it while 
 * it is running. Then, we stop the timer and test. We expect the last value to be not less
 * than the previously recorded values.
 */
TEST(TimerTests, timerGeneralTest) {
    Timer timer;

    ASSERT_EQ(timer.getCurrentTimePeriodDuration(), -1) << "Expected -1, because we have not started the timer";
    ASSERT_EQ(timer.getLastTimePeriodDuration(), -1) << "Expected -1, because we have not started the timer";
    ASSERT_FALSE(timer.isTiming()) << "Expected false, because we have not started the timer";

    timer.startTimer();

    std::chrono::milliseconds dura(50);
    std::this_thread::sleep_for(dura);  // Sleep for 50 ms

    ASSERT_TRUE(timer.isTiming()) << "Expected true, because we have started the timer";
    double time1 = timer.getCurrentTimePeriodDuration();
    ASSERT_GT(time1, 0);
    ASSERT_TRUE(timer.isTiming()) << "Expected true, because we have started the timer";

    std::this_thread::sleep_for(dura);  // Sleep for 50 ms

    double time2 = timer.getCurrentTimePeriodDuration();
    ASSERT_GT(time2, time1);
    ASSERT_TRUE(timer.isTiming()) << "Expected true, because we have started the timer";

    std::this_thread::sleep_for(dura);  // Sleep for 50 ms

    timer.endTimer();
    ASSERT_FALSE(timer.isTiming()) << "Expected false, because we have stopped the timer";

    double time3 = timer.getLastTimePeriodDuration();
    ASSERT_GT(time3, time2);
}