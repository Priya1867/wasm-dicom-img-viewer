#include "GLGraphics.h"
#include "Shader.h"
#include <stdexcept>
#include <string>

namespace {
const char* vertexSrc =
    "#version 300 es\n"
    "layout(location = 0) in vec2 aPosition;\n"
    "layout(location = 1) in vec2 aTexCoord;\n"
    "out vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition, 0.0, 1.0);\n"
    "    vTexCoord = aTexCoord;\n"
    "}\n";

const char* fragmentSrc =
    "#version 300 es\n"
    "precision highp float;\n"
    "precision highp usampler2D;\n"
    "in vec2 vTexCoord;\n"
    "uniform usampler2D uTexture;\n"
    "uniform float uWindow;\n"
    "uniform float uLevel;\n"
    "uniform float uSlope;\n"
    "uniform float uIntercept;\n"
    "out vec4 fragColor;\n"
    "void main() {\n"
    "    uint raw = texture(uTexture, vTexCoord).r;\n"
    "    float intensity = float(raw) * uSlope + uIntercept;\n"
    "    float val = (intensity - (uLevel - 0.5)) / max(uWindow, 1.0) + 0.5;\n"
    "    val = clamp(val, 0.0, 1.0);\n"
    "    fragColor = vec4(val, val, val, 1.0);\n"
    "}\n";
}

GLGraphics::GLGraphics() {
    setupShaders();
    setupBuffers();
}

GLGraphics::~GLGraphics() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
    glDeleteProgram(programID);
    glDeleteTextures(1, &textureID);
}

void GLGraphics::setupShaders() {
    Shader vertex(GL_VERTEX_SHADER, vertexSrc);
    Shader fragment(GL_FRAGMENT_SHADER, fragmentSrc);

    programID = glCreateProgram();
    glAttachShader(programID, vertex.get());
    glAttachShader(programID, fragment.get());
    glLinkProgram(programID);

    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(programID, 512, nullptr, log);
        throw std::runtime_error("Program link error: " + std::string(log));
    }
}

void GLGraphics::setupBuffers() {
    float vertices[] = {
        // positions    // texcoords
        -1.0f, -1.0f,   0.0f, 1.0f,
         1.0f, -1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,   0.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 0.0f
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void GLGraphics::use() const {
    glUseProgram(programID);
}

GLuint GLGraphics::getUniformLocation(const std::string& name) const {
    return glGetUniformLocation(programID, name.c_str());
}

void GLGraphics::uploadTexture(unsigned short* data, int width, int height) {
	
	if (glIsTexture(textureID)) {
		glDeleteTextures(1, &textureID);
	}
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, data);
}

void GLGraphics::bindTexture(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void GLGraphics::draw() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
