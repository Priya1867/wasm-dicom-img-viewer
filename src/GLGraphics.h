#pragma once
#include <GLES3/gl3.h>
#include <string>

class Shader;

class GLGraphics {
public:
    GLGraphics();
    ~GLGraphics();

    void use() const;
    GLuint getUniformLocation(const std::string& name) const;

    void uploadTexture(unsigned short* data, int width, int height);
    void bindTexture(GLenum textureUnit = GL_TEXTURE0) const;
    void draw();

private:
    void setupShaders();
    void setupBuffers();

    GLuint programID;
    GLuint textureID;
    GLuint vao = 0;
    GLuint vbo = 0;
};
