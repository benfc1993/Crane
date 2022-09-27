#pragma once

#include "KeyCodes.h"

namespace Crane
{
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode keycode);
        static bool IsMouseButtonPressed(int button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
}