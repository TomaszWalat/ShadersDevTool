# GLSL Shader Dev Tool
<short description>

 **Functionality**
 <bullet points>
  
<details>
 <summary><b>Picture</b></summary>
<img src="">
</details>

<details>
<summary><b>Created and built with:</b></summary>
  
- OS - Windows 10 Home (v.21H1)
- IDE - Microsoft Visual Studio Community 2019 (v.16.8.3)
- ISO C++ 14 Standard
- OpenGL 4.5 (w/ FreeGlut v.3.0, GLEW v.1.12.0, GLFW v.3.3.4, GLM v.0.9.9.8 - through VS NuGet Package Manager)
- Dear ImGui v1.87
- stb_image v2.23
</details>
  
---
## Developer Walkthrough

<details>
 <summary>Show</summary>
 
[![Project developer walkthrough](https://img.youtube.com/vi/GZvmaqk5AOQ/0.jpg)](https://www.youtube.com/watch?v=GZvmaqk5AOQ)

</details>

---

## Set-up
  
  
---
## Controls
Controls are ignored if the mouse courser is hovering over a GUI panel.
- **`W`** - Move camera forward.
- **`A`** - Move camera Left.
- **`S`** - Move camera backward.
- **`D`** - Move camera right.
- **`Space`** - Move camera up.
- **`Left Shift`** - Move camera down.
- **`E`** - Rotate camera clockwise around screen-plane's Z axis.
- **`Q`** - Rotate camera counter-clockwise around screen-plane's Z axis.
- **`RMB (click'n'drag)`** - Rotate camera around screen-plane's X and Y axis (arcball style). Rotations are capped - -90 to 90 degrees vertically, -180 to 180 horizontally (these caps are adjusted each time RMB is released). Rotation is in the direction the mouse is dragged.
- **`Esc`** - Quite application.

---
## GUI
  

<details>
<summary>Pictures</summary>

<img src="https://user-images.githubusercontent.com/33025239/152699702-8e6b8587-579b-42bc-8f82-5d13d2266bcc.png" alt="Camera data" width="" height="">

<img src="https://user-images.githubusercontent.com/33025239/152699716-4c8495a2-95eb-4c46-9ebf-0170b5dd6f74.png" alt="Object hierarchy" width="" height="">

<img src="https://user-images.githubusercontent.com/33025239/152699710-b7206de0-ac94-4bf2-aa0e-379f9d1e6f06.png" alt="UI Debug Info" width="" height="">
</details>
  
  
  
---
## Architecture
