# GLSL Shader Dev Tool
An OpenGL 4.6 program that presents my custom shader model.

 **Functionality**
- manipulate Model View Projection (MVP) matrices
- control camera position and rotation with keyboard and mouse (arcball style)
- manipulate each scene light individually
- ~~manipulate each scene object individually~~ - UI is curentlly disconnected for object material properties (it'll be back though ;) )
- dynamically change shaders (UI currently disabled) - UI is currently hidded, but the program still uses this functionallity
- there are two versions of the PBR shader - textured and textureless (textureless = adjustable values)
 
**GLSL features**
 - PBR shading model
 - HDR (w/ tone mapping)
 - Bloom effect (using custom Gaussian blur)
 - Skybox (HDR)
 - Multi-light setup
 - Light attenuation for point and spot lights
 - HDR exposure control
 - Skybox reflection / lighting in objects
 - ~~Object base colour (optional)~~ - currently not accessable through UI
 - PBR textures
 - Albedo texturing
 - Roughness texturing
 - Metalic-ness texturing
 - Ambient Occlussion mapping
 - ~~Normal mapping (optional)~~ - temporarly removed
 - ~~Alpha mapping / fragment discarding (optional)~~ - temporarly removed
 
  
<details>
 <summary><b>Picture</b></summary>
 Version 2
<img src="https://user-images.githubusercontent.com/33025239/171000994-17285043-1cf0-453b-b886-ee2339073dec.png" alt="shader scene" width="" height="">
 
 Version 1
<img src="https://user-images.githubusercontent.com/33025239/161029848-3b2a23a1-bd1e-4e20-aad2-3c93cf7e1d08.png" alt="sceneWithUI" width="" height="">
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
<details><summary> What's new </summary>
 - Blinn-Phong has been removed
 - PBR added (textured and textureless version)
 - HDR with tone mapping added
 - Bloom added
 - Skybox changed to HDR
 - Skybox reflection now adds light (linked to brightness of light #0)
 - Light (object) shader
 - new model: sphere
 - new model: cow
 - piggy got removed
 - lots of new PBR textures
 - texturing changed to PBR texturing
</details>
  
---
## Developer Walkthrough


<details>
 <summary>Version 2</summary>
 
[![Project developer walkthrough](https://img.youtube.com/vi/GJkhnt9OuY0/0.jpg)](https://youtu.be/r_n1ZyCDWkY)

</details>

<details>
 <summary>Version 1</summary>
 
[![Project developer walkthrough](https://img.youtube.com/vi/GJkhnt9OuY0/0.jpg)](https://youtu.be/GJkhnt9OuY0)

</details>

---

## Set-up

Things you'll need: 
 - "ShadersDevTool.exe" file
 - "media" folder
 - "shader" folder
 - "imgui.ini" file (technically not needed, however, it stores the position of UI elements)

(Everything listed is included in the release .zip)
 
The files / folders listed needed to be in the same directory - for full structure see architecture section below.
 
Note: Unfortunately I believe there is a memory leak, which (according to analysis tools) seems to be coming from 3rd party files - this will be fixed in the future.
  
---
## Architecture

### Executable version file structure:
- `ShadersDevTool.exe`
- `imgui.ini`
- <details><summary> shader </summary> <!-- folder start -->
 
  - `hdrpbrTexturedShader.vert`
  - `hdrpbrTexturedShader.frag`
  - `hdrpbrTexturelessShader.vert`
  - `hdrpbrTexturelessShader.frag`
  - `lightShader.vert`
  - `lightShader.frag`
  - `skyboxShader.vert`
  - `skyboxShader.frag`
  </details> <!-- end shader folder -->
 
- <details><summary> media </summary> <!-- folder start -->
 
  - `bs_ears.obj`
  - `spot_triangulated.obj`
 
  - <details><summary> 3rd_party </summary> <!-- folder start -->
 
    - <details><summary> freePBR-dot-com </summary> <!-- folder start -->
 
      </details> <!-- end freePBR-dot-com folder -->
  
    - <details><summary> hdrLabs-dot-com </summary> <!-- folder start -->
 
      </details> <!-- end hdrLabs-dot-com folder -->
 
    </details> <!-- end 3rd_party folder -->
 
 
  - <details><summary> texture </summary> <!-- folder start -->
    - <details><summary> ogre </summary> <!-- folder start -->
 
      - `ogre_diffuse.png`
      - `ogre_ao.png`
      </details> <!-- end ogre folder -->
 
    - <details><summary> skybox </summary> <!-- folder start -->
 
      - `lake180_negx.jpg`
      - `lake180_negy.jpg`
      - `lake180_negz.jpg`
      - `lake180_posx.jpg`
      - `lake180_posy.jpg`
      - `lake180_posz.jpg`
      </details> <!-- end skybox folder -->
 
    - <details><summary> cube </summary> <!-- folder start -->
      - <details><summary> pisa-hdr </summary> <!-- folder start -->

       - `lake180_negx.jpg`
       - `lake180_negy.jpg`
       - `lake180_negz.jpg`
       - `lake180_posx.jpg`
       - `lake180_posy.jpg`
       - `lake180_posz.jpg`
       </details> <!-- end pisa-hdr folder -->
      </details> <!-- end cube folder -->
 
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
 
   - Textures (struct) - loads and holds all textures required for the scene.
   - shaders (vector) - lists shader files to compile and link together.
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
 

### Shaders
There are currently three shaders that come with the project:
 - PBR with HDR shader using textures (w/ tone mapping, bloom, light attenuation and skybox reflection)
 - PBR with HDR shader that uses input values (w/ tone mapping, bloom, attenuation and skybox reflection)
 - Skybox shader - for rendering the scene skybox
 - Light shader - for rendering the scene lights
 
Shaders are listed in the `shaders` vector, compiled, linked and placed into the `progs` map - currently the setup does not support geometry shaders, but it will in future versions (it's an easy tweak).
 
Light type is deduced in the shader as follows:
 - if light's `position.w` == 0, then it's a directional light (and calculations for attenuation and cone intensity are nulled)
 - if light's `position.w` == 1 AND `direction` == (0, 0, 0), then light is a point light (cone intensity is nulled, but attenuation is used)
 - if light's `position.w` == 1 AND `direction` != (0, 0, 0), then light is a spotlight (cone intensity and attenuation are used)
 
Currently, for directional lighting, its direction is worked out from its `position.xyz` and not from its `direction` value - this might change in the future, but for now it's this way for the sake of the other light types.

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
 
All panels can be collapsed by clicking the arrow in the top left of the panel. They can be moved around as needed by grabbing them by the top bar. They can also be resized as needed (by grabbing and dragging their bottom right or left corner).

The camera matrices can be reset individually or all together via their respective buttons in the `Camera data` panel.
 
Each object's material can be edited in their individual sections of the `Object Material Info` panel. The `Alpha discard` slider affects only the `teapot` as it's the only object with an alpha texture.
 
Each light can be manipulated in their individual sections of the `Light Info` panel.
 
All values displayed can be manipulated by click 'n' dragging them or double / single clicking them and typing in a value. Edited values directly affect the scene/object.
 
<details>
<summary>Pictures</summary>

<img src="https://user-images.githubusercontent.com/33025239/161029911-28c2ccfe-3290-40bf-bfa1-ae0793f97797.png" alt="Object Material Info" width="" height="">
 
<img src="https://user-images.githubusercontent.com/33025239/161029899-53d7b20f-915e-447c-81ae-ed0d000908a9.png" alt="Light Info" width="" height="">

<img src="https://user-images.githubusercontent.com/33025239/161029882-4829a69c-3682-43dc-a5f4-058d8cb18ff1.png" alt="Camera data" width="" height="">
</details>

 
---
## The scene

In the scene there are 11 objects:
 - the skybox (with mountain lake texture)
 - a plane (with cement texture, called floor)
 - a cube (with brick texture, called box)
 - a pig (on top of the box, called piggy)
 - a cube (with normal map, called matalCube)
 - a torus (with wood texture)
 - an ogre head (with texture and normal map)
 - a teapot (with not texture)
 - a point light
 - and three spot lights
 
<details>
<summary>Pictures</summary>

<img src="https://user-images.githubusercontent.com/33025239/161029866-6c4fa3b4-3133-47c4-abba-a02dc1383f76.png" alt="sceneNoUI" width="" height="">

</details>

The camera's "transform" (like in Unity) is technically split into two: the model (world) matrix which is what is moved to offset the scene's world space (giving the illusion the camera is moving); and the view matrix, which is set back from the origin in the -Z direction (into the screen) and rotated around its axes to give the illusion of the camera rotating around what's in front of it in an arcball fashion.
  

---
## Creadits

PBR textures downloaded from: https://freepbr.com/c/all/
HDR Skyboxes downloaded from: http://www.hdrlabs.com/sibl/archive.html
HDRI to Cubemap converter: https://matheowis.github.io/HDRI-to-CubeMap/


