#include "crpch.h"

#include "OpenGLShader.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Crane
{
    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex") return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
        CR_CORE_ASSERT(false, "Unknown shader type");
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& filePath)
    {
        std::string source = ReadFile(filePath);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);

        auto lastSlash = filePath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;

        auto lastDot = filePath.rfind(".");

        auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
        m_Name = filePath.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
        : m_Name(name)
    {
        std::unordered_map<GLenum, std::string> shaderSources;
        shaderSources[GL_VERTEX_SHADER] = vertexSrc;
        shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
        Compile(shaderSources);
    }
    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererId);
    }

    std::string OpenGLShader::ReadFile(const std::string& filePath)
    {
        std::string result;
        std::ifstream in(filePath, std::ios::in | std::ios::binary);

        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            CR_CORE_ERROR("Could not load file: {0}", filePath);
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;
        CR_CORE_ASSERT(shaderSources.size() <= 2, "Too many shader sources only 2 are supported");

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);

        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            CR_CORE_ASSERT(pos != std::string::npos, "Syntax error");

            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            CR_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextlinePos = source.find_first_of("\r\n", eol);
            pos = source.find(typeToken, nextlinePos);
            shaderSources[ShaderTypeFromString(type)] =
                source.substr(nextlinePos,
                    pos - (nextlinePos == std::string::npos ? source.size() - 1 : nextlinePos));
        }

        return shaderSources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        GLuint program = glCreateProgram();
        std::array<GLenum, 2> glShaderIds;

        int i = 0;

        for (auto& kv : shaderSources)
        {
            GLenum shaderType = kv.first;
            const std::string& source = kv.second;

            GLuint shader = glCreateShader(shaderType);

            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                CR_CORE_ERROR("{0}", infoLog.data());
                CR_CORE_ASSERT(false, "Shader compilation failure");
                break;
            }

            glAttachShader(program, shader);
            glShaderIds[i] = shader;
            i++;
        }

        m_RendererId = program;

        // Link our program
        glLinkProgram(m_RendererId);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_RendererId, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_RendererId, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_RendererId, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_RendererId);

            for (auto id : glShaderIds)
                glDeleteShader(id);

            CR_CORE_ERROR("{0}", infoLog.data());
            CR_CORE_ASSERT(false, "OpenGLShader link failure");
            return;
        }

        // Always detach shaders after a successful link.
        for (auto id : glShaderIds)
            glDetachShader(m_RendererId, id);

    }

    void OpenGLShader::Bind()
    {
        glUseProgram(m_RendererId);
    }
    void OpenGLShader::UnBind()
    {
        glUseProgram(0);
    }


    void OpenGLShader::UploadUniformInt(const std::string& name, const int& value)
    {
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform1i(location, value);
    }
    void OpenGLShader::UploadUniformFloat(const std::string& name, const float& value)
    {
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform1f(location, value);
    }
    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
    {
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform2f(location, values.x, values.y);
    }
    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
    {
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform3f(location, values.x, values.y, values.z);

    }
    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
    {
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }


}
