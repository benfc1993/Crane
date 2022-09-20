#pragma once

#include <random>


namespace Crane {
    class Random
    {
    public:
    public:
        static void Init()
        {
            s_RandomEngine.seed(std::random_device()());
        }

        static float Float()
        {
            return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
        }

    private:
        static std::mt19937 s_RandomEngine;
        static std::uniform_int_distribution<> s_Distribution;
    };
}