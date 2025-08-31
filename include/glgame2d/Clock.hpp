#pragma once

#include <chrono>


class Clock
{
public:
    using TimePoint = std::chrono::high_resolution_clock::time_point;
    
public:
    Clock(float targetFPS);

    /**
     * @brief Get the target FPS (maximum framerate).
     * 
     * @return float The maximum desired framerate
     */
    float getTargetFPS() const;

    /**
     * @brief Set the target FPS (maximum framerate).
     * 
     * @param targetFPS The maximum desired framerate
     */
    void setTargetFPS(float targetFPS);

    /**
     * @brief Compute how much time has passed since the last call, and optionally delay to match the target FPS.
     * This method should only be called once per frame.
     * 
     * @param doDelay Whether to delay (sleep this thread) to match the target FPS
     * @return float The time, in seconds, since the last call (deltaTime).
     */
    float tick(bool doDelay = true);

    /**
     * @brief Get the current time as a TimePoint.
     * 
     * @return TimePoint 
     */
    static TimePoint now();

    /**
     * @brief Get the time elapsed since a TimePoint in microseconds.
     * 
     * @param timePoint 
     * @return std::chrono::microseconds 
     */
    static std::chrono::microseconds timeSince(const TimePoint& timePoint);

private:
    TimePoint m_StartTimePoint;
    float m_TargetFPS;
};
