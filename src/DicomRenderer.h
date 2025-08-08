
// DicomRenderer.h
#pragma once
#include <memory>
#include "GLGraphics.h"

class DicomRenderer {
public:
    DicomRenderer();

    void setWindowLevel(int w, int l);
    void updateTexture(unsigned short* pixels);
    void render();

    int getWindow() const { return window; }
    int getLevel() const { return level; }

    int width = 512;
    int height = 512;
    float slope = 1.0f;
    float intercept = 0.0f;
	bool isMono1 =false;

private:
    int window = 4096;
    int level = 2048;
    std::unique_ptr<GLGraphics> graphics;
};