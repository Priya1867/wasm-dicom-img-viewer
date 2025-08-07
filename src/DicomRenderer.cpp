
// DicomRenderer.cpp
#include "DicomRenderer.h"

DicomRenderer::DicomRenderer() {
    graphics = std::make_unique<GLGraphics>();
}

void DicomRenderer::setWindowLevel(int w, int l) {
    window = w;
    level = l;
    render();
}

void DicomRenderer::updateTexture(unsigned short* pixels) {
    graphics->uploadTexture(pixels, width, height);
}

void DicomRenderer::render() {
    graphics->use();
    graphics->bindTexture();
    glUniform1i(graphics->getUniformLocation("uTexture"), 0);
    glUniform1f(graphics->getUniformLocation("uWindow"), static_cast<float>(window));
    glUniform1f(graphics->getUniformLocation("uLevel"), static_cast<float>(level));
    glUniform1f(graphics->getUniformLocation("uSlope"), slope);
    glUniform1f(graphics->getUniformLocation("uIntercept"), intercept);
    graphics->draw();
}
