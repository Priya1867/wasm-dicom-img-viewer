@echo off
setlocal

REM Set paths
set EMCC=emcc
set OUT_DIR=dcmlite\bin
set DCMLITE_CORE=dcmlite\dcmcore
set DCMLITE_INC=dcmlite
set DCMLITE_CORE_INC=dcmlite\dcmcore
set VIEWER_SRC=src\GLGraphics.cpp src\DicomRenderer.cpp src\DicomLoader.cpp src\main.cpp

echo [1/2] Check for dcmlite dependency...

REM === Check if libdcmlite.a already exists ===
if exist %OUT_DIR%\libdcmlite.a (
    echo libdcmlite.a already exists in %OUT_DIR%
) else (
    echo libdcmlite.a is missing in %OUT_DIR%
    exit /b 1
)

echo Cleaning temp object files...
del /q %TEMP%\emscripten_temp_*.o 2>nul
del /q *.o 2>nul

echo [2/2] Building dicom_viewer WebAssembly module...

%EMCC% %VIEWER_SRC% %OUT_DIR%\libdcmlite.a ^
  -I %DCMLITE_INC% -I %DCMLITE_CORE_INC% ^
  -I src ^
  -s USE_WEBGL2=1 ^
  -s FULL_ES3=1 ^
  -s MIN_WEBGL_VERSION=2 ^
  -s MAX_WEBGL_VERSION=2 ^
  -s ALLOW_MEMORY_GROWTH=1 ^
  -s WASM=1 ^
  -s MODULARIZE=1 ^
  -s EXPORT_ES6=0 ^
  -s EXPORTED_FUNCTIONS=['_malloc','_free'] ^
  -s EXTRA_EXPORTED_RUNTIME_METHODS="['UTF8ToString','stringToUTF8','lengthBytesUTF8']" ^
  -s EXPORT_NAME="DicomModule" ^
  --bind -O3 -o dicom_viewer.js

if errorlevel 1 (
    echo Build failed!
    exit /b 1
)

echo Build completed successfully.

endlocal
pause
