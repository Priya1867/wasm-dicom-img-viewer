#include "DicomLoader.h"
#include "DicomRenderer.h"
#include <dcmcore/dcmcore.h>
#include <string>
#include <sstream>
#include <iostream>
#include <emscripten.h>

int DicomLoader::extractWindowValue(const std::string& str) {
    auto pos = str.find('\\');
    return std::stoi(str.substr(0, pos));
}

bool DicomLoader::loadFromBuffer(uintptr_t ptr, int len, DicomRenderer& renderer) {
    try {
		EM_ASM(console.log("Load Dicom from buffer called"););

        std::string buffer(reinterpret_cast<const char*>(ptr), len);
        std::stringstream stream(buffer);

        dcmcore::DataSet ds;
        dcmcore::TagsReadHandler handler(&ds);

        handler.AddTag({0x7fe0, 0x0010})
               .AddTag({0x0028, 0x0010})
               .AddTag({0x0028, 0x0011})
               .AddTag({0x0028, 0x1050})
               .AddTag({0x0028, 0x1051})
               .AddTag({0x0028, 0x1052})
               .AddTag({0x0028, 0x1053});

        dcmcore::BinaryFile file;
        file.OpenFromStream(stream);

        dcmcore::DicomReader reader(&handler);
        reader.ReadFile(file);

        if (std::string slope_str; ds.GetString({0x0028, 0x1053}, slope_str)) renderer.slope = std::stof(slope_str);
        if (std::string intercept_str; ds.GetString({0x0028, 0x1052}, intercept_str)) renderer.intercept = std::stof(intercept_str);
        if (std::string wl_str; ds.GetString({0x0028, 0x1050}, wl_str)) renderer.setWindowLevel(renderer.getWindow(), extractWindowValue(wl_str));
        if (std::string ww_str; ds.GetString({0x0028, 0x1051}, ww_str)) renderer.setWindowLevel(extractWindowValue(ww_str), renderer.getLevel());

		
		EM_ASM({console.log("Slope: " , $0);}, renderer.slope);
		EM_ASM({console.log("intercept: " , $0);}, renderer.intercept);


        uint16_t w = 512, h = 512;
		if (ds.Get<uint16_t>(dcmcore::Tag(0x0028, 0x0011), w)) { renderer.width = w;}
		if (ds.Get<uint16_t>(dcmcore::Tag(0x0028, 0x0010), h)) {renderer.height = h;}

		EM_ASM({console.log("cols: " , $0);}, w);
		EM_ASM({console.log("rows: " , $0);}, h);

        renderer.width = w;
        renderer.height = h;
		
		EM_ASM({
			var canvas = document.getElementById("dicom-canvas");
			canvas.width = $0;
			canvas.height = $1;
			canvas.style.width = $0 + "px";
			canvas.style.height = $1 + "px";
		}, w, h);

		
        glViewport(0, 0, w, h);

        dcmcore::Buffer pixBuf;
        std::size_t length;
        ds.GetBuffer({0x7fe0, 0x0010}, pixBuf, length);
		EM_ASM(console.log("Pixel data length:", $0);, length);


        renderer.updateTexture(reinterpret_cast<unsigned short*>(pixBuf.data()));
        renderer.render();
        return true;
    } catch (...) {
        std::cerr << "DICOM load failed" << std::endl;
        return false;
    }
}
