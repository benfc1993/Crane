#ifdef __linux__
#include "crpch.h"

#include "Crane/Utils/PlatformUtils.h"

#define LINUX_PATH_MAX 4096 + 255 + 1

namespace Crane {
    std::string FileDialogs::OpenFile()
    {
        char filename[LINUX_PATH_MAX];
        FILE* f = popen("zenity --file-selection --modal --title=\"Open Scene\"", "r");

        while (fgets(filename, LINUX_PATH_MAX, f) != NULL)
        {
        }

        strtok(filename, "\n");
        std::string result(filename);
        if (result.find('/') == std::string::npos)
        {
            pclose(f);
            return std::string();
        }

        pclose(f);

        return result;
    }

    std::string FileDialogs::SaveFile()
    {
        char filename[LINUX_PATH_MAX];
        FILE* f = popen("zenity --file-selection --save --modal --title=\"Save Scene\"", "r");

        while (fgets(filename, LINUX_PATH_MAX, f) != NULL)
        {
        }

        strtok(filename, "\n");
        std::string result(filename);
        if (result.find('/') == std::string::npos)
        {
            pclose(f);
            return std::string();
        }

        pclose(f);
        return result;
    }
}

#endif

