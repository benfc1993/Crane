#pragma once

namespace Crane {
    class Time
    {
    public:
        Time(float time = 0.0f, float deltaTime = 0.0f) : m_Time(time), m_DeltaTime(deltaTime) {}

        operator float() const { return m_Time; };
        float GetTime() const { return m_Time; };
        float GetMilliseconds() const { return m_Time * 1000.0f; };
        float DeltaTime() const { return m_DeltaTime; };

    private:
        float m_DeltaTime;
        float m_Time;
    };
}