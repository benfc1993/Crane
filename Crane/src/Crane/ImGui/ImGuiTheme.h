#pragma once

namespace Crane {
    struct ImGuiTheme
    {
        float Primary[4]{ 0.226f, 0.721f, 0.755f, 1.000f };  //#3BF3FF
        float PrimaryLight[4]{ 0.117f, 0.817f, 0.869f, 1.000f }; //#00EEFF
        float PrimaryDark[4]{ 0.218f, 0.524f, 0.549f, 1.000f }; //#77F5FF
        float Secondary[4]{ 0.96f, 0.38f, 0.0f, 1.0f };
        float White[4]{ 0.98, 1.0f, 1.0f, 1.0f }; //#F9FFFF
        float Dark[4]{ 0.150f, 0.167f, 0.190f, 1.000f }; //#38475C
        float Medium[4]{ 0.167f, 0.197f, 0.236f, 1.000f }; //#414F63
        float Light[4]{ 0.289f, 0.322f, 0.359f, 1.000f }; //#5C738D
        float WindowBg[4]{ 0.135f, 0.132f, 0.139f, 1.000f }; //#A87779
    };
}