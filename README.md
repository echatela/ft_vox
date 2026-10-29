> 🚧 **Work in progress.** This README is an intermediate version, meant to present the project while it is still being developed. The engine core works (chunk streaming, rendering, interface); procedural terrain generation is being developed on a separate branch, so the current world is flat.

# ft_vox

[![C++](https://img.shields.io/badge/language-C++26-555?style=flat-square&logo=cplusplus)](https://en.cppreference.com/w/cpp)
![OpenGL](https://img.shields.io/badge/OpenGL-4.6-555?style=flat-square&logo=opengl)
![Status](https://img.shields.io/badge/status-in%20progress-orange?style=flat-square)

A Minecraft-style voxel engine in modern C++ and OpenGL, focusing on real-time rendering and on streaming a large world around the player.

---

## Build & Run

### Requirements
- Linux
- `clang++` with C++26 support (clang 18 or later), `make`
- GLFW 3 (`libglfw3-dev` on Ubuntu, `glfw-devel` on Fedora)
- A GPU and driver supporting **OpenGL 4.6**

GLAD, GLM and stb_image are included in `external/`.

> Note: ft_vox has been developed on **Fedora Linux 44**.

### Compilation
`make`

### Run
`./ft_vox`

---

## Controls

- **W / A / S / D** : Move
- **Space** : Move up
- **Left Shift** : Sprint
- **Mouse** : Look around
- **1 – 9** : Select a hotbar slot
- **F5** : Toggle the debug overlay (FPS, position, loaded chunks)
- **Esc** : Quit

---

## Technical Overview

### Features

- **Chunked world** : chunks of 16 × 16 × 256 blocks, loaded in a 21 × 21 area around the player (441 chunks)
- **Chunk streaming** : when the player crosses a chunk border, the row of chunks left behind is recycled for the row ahead, reusing the same allocations instead of freeing and reallocating
- **Meshing with hidden-face removal** : only the block faces next to an empty block (or on a chunk border) are sent to the GPU
- **Textures** : all block textures in a single texture array, with mipmaps
- **Resource manager** : central loading of shaders, textures and fonts
- **Scene graph** : nodes with transforms, used for the world and the interface
- **Interface** : controls anchored to 9 screen positions (corners, edges, center), bitmap font labels, hotbar, debug overlay
- **Frame-rate independent movement** (delta time)

### In Progress
- Procedural terrain generation with fractal noise (`feat/ProcGen` branch)
- Chunk meshing aware of neighbouring chunks (`refacto/chunk_manager_load_chunk` branch)
- Multithreaded chunk generation (planned)

### Architecture

- **Application** : `src/app/`
  - Main loop: input, update, render
- **Platform** : `src/platform/`
  - GLFW window and OpenGL context (GLAD)
- **Rendering** : `src/render/`
  - Shaders, 2D textures and texture arrays
- **Resources** : `src/loader/`
  - Resource manager
- **Scene** : `src/scene/`
  - Scene graph nodes, interface controls, labels, 2D meshes
- **World** : `src/world/`
  - Camera, chunks and chunk manager
- **Shaders** : `shaders/`
  - GLSL shaders for chunks, interface and text

---

## More

### Collaboration

This project is developed by:
- **Theo Fiette** / [theofiette](https://github.com/theofiette)
- **Edouard** / [echatela](https://github.com/echatela)

### Educational Context

This project is developed as part of a programming curriculum (42 Paris) and aims to demonstrate:

- Real-time 3D rendering with modern OpenGL
- Managing a large, streamed world with bounded memory
- Engine architecture: resources, scene graph, interface
- Teamwork on a shared C++ codebase

> Note: This project is for educational purposes, as a hands-on learning experience for graphics and engine programming.

### AI Usage

This README was written with the help of Claude (Anthropic).

For the project itself, we agreed not to rely on AI. We also avoid tutorials on how to build a voxel engine: the goal is to learn by ourselves.
