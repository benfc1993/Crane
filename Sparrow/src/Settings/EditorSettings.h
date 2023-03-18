#pragma once 

#include "Crane/Core/Base.h"

namespace Crane {


    struct EditorSettingsData
    {
        bool ShowColiders = true;

        EditorSettingsData() = default;
    };


    class EditorSettings
    {
    public:
        EditorSettings();
        ~EditorSettings();
        static void Serialize();
        static bool Deserialize();

        static EditorSettingsData* Get();

    };
}