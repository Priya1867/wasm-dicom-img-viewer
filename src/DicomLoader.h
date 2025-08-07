#pragma once
#include <cstdint>
#include <string>

class DicomRenderer;

class DicomLoader {
public:
    bool loadFromBuffer(uintptr_t dataPtr, int length, DicomRenderer& renderer);

private:
    int extractWindowValue(const std::string& str);
};
