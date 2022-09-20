#include "Random.h"

namespace Crane {
    std::mt19937 Random::s_RandomEngine;
    std::uniform_int_distribution<> Random::s_Distribution;
}