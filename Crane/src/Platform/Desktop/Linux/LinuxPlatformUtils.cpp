#ifdef __linux__
#include "crpch.h"

#include "Crane/Utils/PlatformUtils.h"

namespace Crane {
    std::string FileDialogs::OpenFile()
    {
        char filename[1024];
        FILE* f = popen("zenity --file-selection ", "r");
        while (fgets(filename, 1024, f) != NULL)
        {

        }

        pclose(f);

        strtok(filename, "\n");
        std::string result(filename);
        return result;
    }

    std::string FileDialogs::SaveFile()
    {
        char filename[1024];
        FILE* f = popen("zenity --file-selection --save", "r");
        while (fgets(filename, 1024, f) != NULL)
        {

        }

        pclose(f);
        CR_CORE_INFO(filename);
        strtok(filename, "\n");
        std::string result(filename);
        return result;
    }
}

#endif