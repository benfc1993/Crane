#pragma once

#include <string>

namespace Crane
{
    class Shader
    {
    public:
        Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
        ~Shader();

        void Bind();
        void UnBind();

    private:
        uint32_t m_RendererId;
    };
}