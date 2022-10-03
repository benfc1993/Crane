#pragma once

#include <string>

namespace Crane {

    class FileDialogs
    {
    public:
        // These return empty strings if cancelled
        static std::string OpenFile();
        static std::string SaveFile();
    };
}