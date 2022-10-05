#include "crpch.h"

#include "Crane/Utils/PlatformUtils.h"

namespace Crane {
#ifdef __linux__
    std::string FileDialogs::OpenFile()
    {
        char filename[1024];
        FILE* f = popen("zenity --file-selection --modal --title=\"Open Scene\"", "r");
        while (fgets(filename, 1024, f) != NULL)
        {
        }

        pclose(f);
        std::string str(filename);
        if (str.find('/') == std::string::npos)
        {
            return std::string();
        }
        strtok(filename, "\n");
        std::string result(filename);
        return result;
    }

    std::string FileDialogs::SaveFile()
    {
        char filename[1024];
        FILE* f = popen("zenity --file-selection --save --modal --title=\"Save Scene\"", "r");
        while (fgets(filename, 1024, f) != NULL)
        {

        }
        pclose(f);
        std::string str(filename);
        if (str.find('/') == std::string::npos)
        {
            return std::string();
        }

        strtok(filename, "\n");
        std::string result(filename);
        return result;
    }
#endif
}


