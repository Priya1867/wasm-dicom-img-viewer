# 🖼️ WebAssembly DICOM Image Viewer

This project is a **lightweight, browser-based DICOM viewer** written in C++ and compiled to WebAssembly using **Emscripten**. It uses **WebGL2 shaders** to render grayscale medical images with support for **window/level (W/L)** interaction.

---

## 🚀 Features

- ✅ Load and display DICOM images directly in the browser
- ✅ Pure C++ backend with dcmlite-based DICOM parsing
- ✅ WebGL2 shaders for real-time intensity rendering
- ✅ Mouse-based window/level control
- ✅ Works offline — no backend server required
- ✅ Modular C++ code structure

---

## 🛠️ Building the Project

You must have [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) installed and activated.

```bash
> build.bat
```

This will:
- Compile all C++ files into `dicom_viewer.js` + `dicom_viewer.wasm`

---

## ▶️ Running It

Just open `index.html` in a browser (Chrome/Firefox with WebAssembly + WebGL2 support):

```bash
> start index.html
```

Or use a simple local server:

```bash
python -m http.server 8080
```

---

## 🧪 Usage

1. Click **Upload DICOM**
2. Select a `.dcm` file
3. Use your mouse:
   - Drag horizontally → adjusts **Window**
   - Drag vertically → adjusts **Level**

W/L values will be shown below the image.

---

## 📄 License

MIT License. Use freely, contribute openly.

---

## 🙌 Credits

Developed by [Priya Ravichandran]  
DICOM parsing powered by [dcmlite](https://github.com/feliwir/dcmlite)
