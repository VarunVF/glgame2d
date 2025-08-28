#include "glgame2d/Clock.hpp"

#include <chrono>
#include <thread>


Clock::Clock(float targetFPS)
    : m_TargetFPS{ targetFPS }
{
    m_StartTimePoint = now();
}

float Clock::getTargetFPS() const
{
    return m_TargetFPS;
}

void Clock::setTargetFPS(float targetFPS)
{
    m_TargetFPS = targetFPS;
}

float Clock::tick(bool doDelay)
{
    auto frameTimeUS = std::chrono::duration_cast<std::chrono::microseconds>(now() - m_StartTimePoint);
    const float desiredFrameTimeUS = 1.0f / m_TargetFPS * 1.0e6f;

    float sleepTime = desiredFrameTimeUS - frameTimeUS.count();
    if (doDelay && sleepTime > 0)
    {
        auto delay = std::chrono::microseconds(static_cast<int>(sleepTime));
        std::this_thread::sleep_for(delay);

        // Factor in the delay
        frameTimeUS = std::chrono::duration_cast<std::chrono::microseconds>(now() - m_StartTimePoint);
    }

    m_StartTimePoint = now();
    float deltaTime = frameTimeUS.count() / 1.0e6f;
    return deltaTime;
}

std::chrono::high_resolution_clock::time_point Clock::now()
{
    return std::chrono::high_resolution_clock::now();
}
