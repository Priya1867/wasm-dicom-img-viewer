// main.cpp
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/bind.h>
#include "DicomRenderer.h"
#include "DicomLoader.h"
#include <iostream>

std::unique_ptr<DicomRenderer> renderer;
std::unique_ptr<DicomLoader> loader;

void SetWindowLevel(int w, int l) {
    if (renderer) renderer->setWindowLevel(w, l);
}

void LoadDicom(uintptr_t ptr, int len) {
    if (loader && renderer) { 
		loader->loadFromBuffer(ptr, len, *renderer);
	}
}

void Render() {
    if (renderer) renderer->render();
}

int getWWidth() {
    return renderer ? renderer->getWindow() : 0;
}

int getWCenter() {
    return renderer ? renderer->getLevel() : 0;
}
int InitViewer() {
	
    EmscriptenWebGLContextAttributes attr;
    emscripten_webgl_init_context_attributes(&attr);
	attr.alpha = false;
	attr.depth = false;
	attr.stencil = false;
	attr.antialias = false;
	attr.majorVersion = 2;
	attr.minorVersion = 0;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#dicom-canvas", &attr);
	 if (ctx <= 0) {
		 EM_ASM(console.log("Failed to create WebGL2 context"););
        std::cerr << "Failed to create WebGL2 context. Handle = " << ctx << std::endl;
        return 1; // Return error
    }

    emscripten_webgl_make_context_current(ctx);
	
    renderer = std::make_unique<DicomRenderer>();
    loader = std::make_unique<DicomLoader>();

    return 0;
}

EMSCRIPTEN_BINDINGS(my_module) {
	emscripten::function("InitViewer", InitViewer);
    emscripten::function("SetWindowLevel", SetWindowLevel);
    emscripten::function("LoadDicom", LoadDicom, emscripten::allow_raw_pointers());
    emscripten::function("Render", Render);
    emscripten::function("getWWidth", getWWidth);
    emscripten::function("getWCenter", getWCenter);
}
