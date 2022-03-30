# GLSL Shader Dev Tool
<short description>

 **Functionality**
- manipulate Model View Projection (MVP) matricies;
- control camera position and rotation with keyboard and mouse (arcball style);
- manipulate each scene light individually;
- manipulate each scene object individually;
 
**GLSL features**
 - Blinn-Phong shading
 - Skybox
 - Multi-light setup
 - Light attenuation for point and spot lights
 - Skybox reflection in objects
 - Object base colour (optional)
 - Albedo texturing (optional)
 - Normal mapping (optional)
 - Alpha mapping / fragment discarding (optional)
 
  
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
 
Note: Unfortunatlly I believe there is a slight memory leak, which (according to analysis tools) seems to be coming from 3rd party files - this will be fixed in the future.
  
---
## Architecture

### Executable version file structure:
- `ShadersDevTool.exe`
- `imgui.ini`
- <details><summary> shader </summary> <!-- folder start -->
 
  - `blinnPhongShader.vert`
  - `blinnPhongShader.frag`
  - `blinnPhongShader_normalMap.vert`
  - `blinnPhongShader_normalMap.frag`
  - `skyboxShader.vert`
  - `skyboxShader.frag`
  </details> <!-- end shader folder -->
 
- <details><summary> media </summary> <!-- folder start -->
 
  - `bs_ears.obj`
  - `pig_triangulated.obj`
 
  - <details><summary> texture </summary> <!-- folder start -->
 
    - `cement.jpg`
    - `fire.png`
    - `star.png`
 
    - <details><summary> brick </summary> <!-- folder start -->
 
      - `brick1.jpg`
      </details> <!-- end brick folder -->
 
    - <details><summary> ogre </summary> <!-- folder start -->
 
      - `ogre_diffuse.png`
      - `ogre_normalmap.png`
      </details> <!-- end ogre folder -->
 
    - <details><summary> ripple </summary> <!-- folder start -->
 
      - `NormalMap_invertedR.png`
      </details> <!-- end ripple folder -->
 
    - <details><summary> skybox </summary> <!-- folder start -->
 
      - `lake180_negx.jpg`
      - `lake180_negy.jpg`
      - `lake180_negz.jpg`
      - `lake180_posx.jpg`
      - `lake180_posy.jpg`
      - `lake180_posz.jpg`
      </details> <!-- end skybox folder -->
 
    - <details><summary> wood </summary> <!-- folder start -->
 
      - `Albedo.jpg`
      </details> <!-- end wood folder -->
 
    </details> <!-- end texture folder -->
  </details> <!-- end media folder -->
 
 ### Internal program structure:
 - <details><summary> main.cpp </summary> 
 
   - `main()` - Program entry point. Creates instance of SceneRunner and Scene, passes scene into scenerunner via `run()`.
 </details>
 
 - <details><summary> Scene </summary>
 
   - defines Scene functionality and Camera cam.
 </details>
 
 - <details><summary> SceneRunner </summary> 
 
   - `SceneRunner()` - sets up program window and Dear ImGui, handles programs 3D scene.
   - `run()` - calls Scene's `setDimensions()`, `initScene()`, `resize()`, and `mainLoop()` (passing it the scene). Then, once `mainloop()` returns control, it shuts down Dear ImGui and glfw (program window), and terminates program.
   - `mainLoop()` - keeps the program control until loop's exit (window closed or space bar is pressed). While in loop, starts new GUI frame, calls scene's `update()` then `render()`, calls `debugGUI()`, renders GUI frame, swaps GLFW buffers, and finally, polls GLFW events and calls `processInput()`.
   - `debugGUI()` - injects debug info panel into GUI frame (for now it's just some mouse data).
   - `processInput()` - processes GLFW window keyboard and mouse input events and calls appropriate functions.
 </details>
 
 - <details><summary> SceneBasic_Uniform (extends Scene)</summary>
 
   - Textures (struct) - loads  and holds all textures required for the scene.
   - shaders (vector) - lists shader files to compile and link togather.
   - `SceneBasic_Uniform()` - initialises objects in scene (skybox, floor, metalCube, box, torus, teapot, piggy, ogre).
   - `initScene()` - initialises camera (cam), calls `compile()`, maps textures to objects, positions objects in scene, sets objects' material info, populates lights vector (initialises 4 lights).
   - `compile()` - compiles and links shaders as specified in shaders vector (as GLSLProgram objects), and places them into progs map.
   - `changeShader()` - changes the active shader program.
   - `setMatrices()` - updates MVP related uniforms in active shader.
   - `setMeshUniforms()` - updates object specific uniforms (material info, object position) in active shader, based on the object it's passed.
   - `setLights()` - updates each light's uniform data in active shader.
   - `drawGUI()` - injects light info and object material panels into GUI frame.
   - `update()` - updates the scene (e.g. animation of movement)
   - `render()` - calls `drawGUI()`, each object's `render()`, and sets active shader's uniforms via setMatrices(), setLights() and setMeshUniform(); here is also where `changeShader()` is called if different shaders are used for different objects.
 </details>
 
 - <details><summary> Camera </summary>
 
   -  controls the scene's camera, contains the scene's MVP.
 </details>
 
 - <details><summary> MaterialInfo (struct) </summary>
 
   - material info container
 </details>
 
 - <details><summary> LightInfo (struct) </summary>
 
   - light info container
 </details>

 - <details><summary> SceneObject </summary>
 
   - contains object specific model matrix and material info.
 </details>
 
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
  
Panels:
 - `Camera data` - Displays the camera's Model-View-Projection matrices and debug info (which includes the View's inverse).
 - `Object Material Info` - Displays all objects and their material info.
 - `Light info` - Displays all lights and their info.
 - `Debug info` - Displays window/viewport/mouse related data.
 
All panels can be collapsed by clicking the arrow in the top left of the panel. They can be moved around as needed by grapping them by the top bar. They can also be resized as needed (by grapping and dragging their bottom right or left corner).

The camera matrices can be reset individually or all together via their respective buttons in the `Camera data` panel.
 
Each object's material can be edited in their individual sections of the `Object Material Info` panel.
 
Each light can be manipulated in their individual sections of the `Light Info` panel.
 
All values displayed can be manipulated by click 'n' dragging them or double / single clicking them and typing in a value. Edited values directly affects the scene/object.
 
<details>
<summary>Pictures</summary>

<img src="" alt="Camera data" width="" height="">

<img src="" alt="Object Material Info" width="" height="">
 
<img src="" alt="Light Info" width="" height="">

<img src="" alt="Debug Info" width="" height="">
</details>
 
 
---
## The scene


<details>
<summary>Pictures</summary>

<img src="" alt="" width="" height="">

<img src="" alt="" width="" height="">

<img src="" alt="" width="" height="">
</details>

  
