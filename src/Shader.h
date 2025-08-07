// Shader.h
#pragma once
#include <GLES3/gl3.h>
#include <stdexcept>

class Shader {
public:
    Shader(GLenum type, const char* source) {
        shaderID = glCreateShader(type);
        glShaderSource(shaderID, 1, &source, nullptr);
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            char log[512];
            glGetShaderInfoLog(shaderID, 512, nullptr, log);
            throw std::runtime_error("Shader compile error: " + std::string(log));
        }
    }

    ~Shader() {
        glDeleteShader(shaderID);
    }

    GLuint get() const { return shaderID; }

private:
    GLuint shaderID;
};
