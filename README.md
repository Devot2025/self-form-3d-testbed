# self-form-3d-testbed

A testbed for exploring self-forming and self-organizing 3D geometries using computational and topological approaches.

---

## Overview

This repository contains code for **loading and running a self-forming 3D file format**.

The meshes used here are simple, and the animations may look somewhat unusual.  
However, the core logic itself is likely correct and has not changed since the initial implementation.

The `.sc3` files are **created and tested in Blender**, and the same data is used directly in this project.  
In Blender, the files behave as intended.

---

## Requirements

The following are required **before building and running this project**:

- **OpenGL**
- **GLFW**

---

## Self-Forming 3D Format

- The self-forming 3D file format uses the extension **`.sc3`**
- The example used in this repository is **`a.sc3`**
- The format is experimental, and its specification may change in the future

---

## Screenshot

Below is an example of rendering and playing back `a.sc3` using this code.

![Screenshot](image/test_1.png)

---

## Notes

- This project is intended for **experimental and research purposes**
- Visual quality (mesh detail and animation smoothness) is secondary to  
  **correctly interpreting and executing the self-forming data**
