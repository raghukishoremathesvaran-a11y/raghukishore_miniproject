# ASCII Graphics Drawing Application in C

A modular C-based command-line program that allows users to interactively draw, modify, delete, and render geometric shapes onto a dynamic 2D array of characters (an ASCII art canvas).

## Features

- **Dynamic Canvas**: Create a canvas of custom dimensions with a custom background character.
- **Multiple Shapes Supported**:
  - **Line**: Drawn using Bresenham's line algorithm.
  - **Rectangle**: Support for outline or filled rendering.
  - **Circle**: Drawn using the Midpoint/Bresenham circle algorithm (supports filled or outline).
  - **Triangle**: Drawn as an outline or filled using a vertical-splitting scanline algorithm.
- **CRUD Operations**: Add new shapes, list active shapes, modify existing shapes (keeping current properties as defaults by pressing Enter), or delete shapes.
- **Border Visualizer**: Renders the canvas wrapped inside a clean border box.
- **Automated Tests**: Unit test suite verifying all canvas operations and shape rasterization.

---

## File Structure

- `main.c`: Contains the user interactive shell interface, parsing functions, and menu workflows.
- `graphics.c`: Implements the Canvas manager and all math algorithms to rasterize shapes.
- `graphics.h`: Defines structures (`Canvas`, `Shape`, shape data) and function signatures.
- `test_graphics.c`: Contains the automated unit tests.
- `build.bat`: Windows batch script to compile the application and tests.
- `.gitignore`: Configured to prevent compiled binaries (`.exe` / `.obj`) from being tracked in Git.

---

## Building and Running

### Prerequisites
To compile the C source files, you need a C compiler installed (such as `gcc` or `clang`). 

### Using the Build Script
If you are on Windows, you can compile the project using the included build script:
```powershell
.\build.bat
```

### Manual Compilation
You can compile the files manually using `gcc`:
```bash
# Compile the main application
gcc -O2 -o main.exe main.c graphics.c -lm

# Compile the test suite
gcc -O2 -o test_graphics.exe test_graphics.c graphics.c -lm
```

---

## Running the Application

### 1. Launch the App
Start the compiled executable:
```powershell
.\main.exe
```

### 2. Run the Unit Tests
Execute the unit tests to check the algorithms' correctness:
```powershell
.\test_graphics.exe
```

---


```
# raghukishore_miniproject
