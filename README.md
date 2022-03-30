# GLSL Shader Dev Tool
<short description>

 **Functionality**
- manipulate Model View Projection (MVP) matricies;
- control camera position and rotation with keyboard and mouse (arcball style);
- manipulate each scene light individually;
- manipulate each scene object individually;
 
  
<details>
 <summary><b>Picture</b></summary>
<img src="">
</details>

<details>
<summary><b>Created and built with:</b></summary>
  
- OS - Windows 10 Home (v.21H1)
- IDE - Microsoft Visual Studio Community 2019 (v.16.8.3)
- ISO C++ 14 Standard
- OpenGL 4.6 (w/ Glad v.0.1.35 GLFW v.3.3.6, GLM v.0.9.8; GLFW built using CMake v.3.22.2)
- Dear ImGui v.1.87 WIP
- stb_image v.2.19
</details>
  
---
## Developer Walkthrough

<details>
 <summary>Show</summary>
 
[![Project developer walkthrough]()]()

</details>

---

## Set-up

Things you'll need: 
 - "ShadersDevTool.exe" file
 - "media" folder
 - "shader" folder
 - "imgui.ini" file (techincially not needed, however, it stores the position of UI elementes)

(Everything listed is included in the release .zip)
 
The files / folders listed needed to be in the same directory - for full structure see architecture section below.
 
  
---
## Architecture

Executable version structure:
- `ShadersDevTool.exe`
- `imgui.ini`
- shader (folder) 
  - `blinnPhongShader.vert`
  - `blinnPhongShader.frag`
  - `blinnPhongShader_normalMap.vert`
  - `blinnPhongShader_normalMap.frag`
  - `skyboxShader.vert`
  - `skyboxShader.frag`
- media (folder)
  - `bs_ears.obj`
  - `pig_triangulated.obj`
  - texture (folder)
    - `cement.jpg`
    - `fire.png`
    - `star.png`
    - brick (folder)
      - `brick1.jpg`
    - ogre (folder)
      - `ogre_diffuse.png`
      - `ogre_normalmap.png`
    - ripple (folder)
      - `NormalMap_invertedR.png`
    - skybox (folder)
      - `lake180_negx.jpg`
      - `lake180_negy.jpg`
      - `lake180_negz.jpg`
      - `lake180_posx.jpg`
      - `lake180_posy.jpg`
      - `lake180_posz.jpg`
    - wood (folder)
      - `Albedo.jpg`
 
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

<img src="" alt="Camera data" width="" height="">

<img src="" alt="Object hierarchy" width="" height="">

<img src="" alt="UI Debug Info" width="" height="">
</details>
  
  
