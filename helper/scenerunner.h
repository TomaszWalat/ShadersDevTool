#pragma once

//#include <glad/glad.h>
#include "scene.h"
//#include <GLFW/glfw3.h>
#include "glutils.h"

#include <map>
#include <string>
#include <fstream>
#include <iostream>


#include "../imgui/imgui.h"
#include "../imgui/backend/imgui_impl_glfw.h"
#include "../imgui/backend/imgui_impl_opengl3.h"

#define WIN_WIDTH 1600
#define WIN_HEIGHT 900
#define GLSL_VERSION "#version 460"

// Mouse cursor data
struct MouseData {
	float clickPoint_x;
	float clickPoint_y;
	float clickVector_x;
	float clickVector_y;
	float position_x;
	float position_y;
	float previous_x;
	float previous_y;
	float delta_x;
	float delta_y;
    bool hoveringOverUI;
} mouse;

class SceneRunner {
private:
    GLFWwindow* window;
    ImGuiIO* io;
    int fbw, fbh;
	bool debug;           // Set true to enable debug messages

public:
    SceneRunner(const std::string & windowTitle, int width = WIN_WIDTH, int height = WIN_HEIGHT, int samples = 0) : debug(true) {
        // Initialize GLFW
        if( !glfwInit() ) exit( EXIT_FAILURE );

#ifdef __APPLE__
        // Select OpenGL 4.1
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
#else
        // Select OpenGL 4.6
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
#endif
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        if(debug) 
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        if(samples > 0) {
            glfwWindowHint(GLFW_SAMPLES, samples);
        }

        // Open the window
        window = glfwCreateWindow( WIN_WIDTH, WIN_HEIGHT, windowTitle.c_str(), NULL, NULL );
        if( ! window ) {
			std::cerr << "Unable to create OpenGL context." << std::endl;
            glfwTerminate();
            exit( EXIT_FAILURE );
        }
        glfwMakeContextCurrent(window);

        // Get framebuffer size
        glfwGetFramebufferSize(window, &fbw, &fbh);

        // Set GLFW input settings
		glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
		glfwSetCursorPosCallback(window, cursor_position_callback);

        // Load the OpenGL functions.
        if(!gladLoadGL()) { exit(-1); }

        // Setup Dear ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();                                     // Create Dear ImGui context
        /*ImGuiIO&*/ io = &ImGui::GetIO(); (void)io;                // Get Dear ImGui input interface
        ImGui::StyleColorsDark();                                   // Set GUI theme / colour scheme
		ImGui_ImplGlfw_InitForOpenGL(window, true);    // Dear ImGui <-> OpenGL interface 
        ImGui_ImplOpenGL3_Init(GLSL_VERSION);             // Dear ImGui <-> GLFW interface

        GLUtils::dumpGLInfo();

        // Initialization
        glClearColor(0.5f,0.5f,0.5f,1.0f);
#ifndef __APPLE__
		if (debug) {
			glDebugMessageCallback(GLUtils::debugCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
			glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
				GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Start debugging");
		}
#endif
    }

    int run(Scene & scene) {
        scene.setDimensions(fbw, fbh);
        scene.initScene();
        scene.resize(fbw, fbh);

        // Enter the main loop
        mainLoop(window, scene);

#ifndef __APPLE__
		if( debug )
			glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 1,
				GL_DEBUG_SEVERITY_NOTIFICATION, -1, "End debug");
#endif

        //delete io;

        // Clean up Dear ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

		// Close window and terminate GLFW
		glfwTerminate();

        //int ch = getchar();

        // Exit program
        return EXIT_SUCCESS;
    }

    static std::string parseCLArgs(int argc, char ** argv, std::map<std::string, std::string> & sceneData) {
        if( argc < 2 ) {
            printHelpInfo(argv[0], sceneData);
            exit(EXIT_FAILURE);
        }

        std::string recipeName = argv[1];
        auto it = sceneData.find(recipeName);
        if( it == sceneData.end() ) {
            printf("Unknown recipe: %s\n\n", recipeName.c_str());
            printHelpInfo(argv[0], sceneData);
            exit(EXIT_FAILURE);
        }

        return recipeName;
    }

private:
    static void printHelpInfo(const char * exeFile,  std::map<std::string, std::string> & sceneData) {
        printf("Usage: %s recipe-name\n\n", exeFile);
        printf("Recipe names: \n");
        for( auto it : sceneData ) {
            printf("  %11s : %s\n", it.first.c_str(), it.second.c_str());
        }
    }

    void mainLoop(GLFWwindow* window, Scene& scene) {

        while( ! glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) ) {
            GLUtils::checkForOpenGLError(__FILE__,__LINE__);

            // Begin Dear ImGui new frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Update scene (time since last loop run)
            scene.update(float(glfwGetTime()));

            // Render scene
            scene.render();

            debugGUI();
            
            // Render and drew Dear ImGui frame
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
            processInput(scene);
        }
    }

    void processInput(Scene& scene)
    {
    
		int state = glfwGetKey(window, GLFW_KEY_SPACE);
		if (state == GLFW_PRESS)
			scene.animate(!scene.animating());
            

		// For running code only once when RMB is pressed - persistent between function calls
		static bool rmbPressedFirstTime = false; 
		// For running code only once after RMB is realeased - persistent between function calls
		static bool rmbReleased = true;

		// Ignore inputs if mouse is hovering over UI
		if (!io->WantCaptureMouse)
		{
			mouse.hoveringOverUI = false;

			// Camera three dimensional movement
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) scene.cam->moveRight();
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) scene.cam->moveLeft();
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) scene.cam->moveUp();
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) scene.cam->moveDown();
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) scene.cam->moveBackward();
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) scene.cam->moveForward();

			// Camera tilting 
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) scene.cam->tilt(1.0f);
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) scene.cam->tilt(-1.0f);

			// Camera arcball style rotation
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				rmbReleased = false;

				if (!rmbPressedFirstTime) // Executes only once when RMB is pressed
				{
					rmbPressedFirstTime = true;

					mouse.clickPoint_x = mouse.position_x;
					mouse.clickPoint_y = mouse.position_y;

					scene.cam->updateInverse(); // Update camera's rotation point of reference

					//std::cout << "RMB was pressed - first call" << std::endl;
				}

				if (mouse.delta_x != 0.0f || mouse.delta_y != 0.0f) // Only run if mouse has moved
				{
					mouse.clickVector_x = mouse.position_x - mouse.clickPoint_x;
					mouse.clickVector_y = mouse.position_y - mouse.clickPoint_y;

					scene.cam->rotateAroundTarget(glm::vec2(mouse.delta_x, mouse.delta_y));
				}
			}

			// Executes only once when RMB is released
			if (!rmbReleased && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
			{
				// Reset flags and mouse data
				rmbReleased = true;
				rmbPressedFirstTime = false;

				mouse.clickPoint_x = 0.0f;
				mouse.clickPoint_y = 0.0f;

				mouse.clickVector_x = 0.0f;
				mouse.clickVector_y = 0.0f;

				scene.cam->updateInverse(); // Update camera's rotation point of reference

				//std::cout << "RMB was released" << std::endl;
			}
		}
		else
		{
			mouse.hoveringOverUI = true;
		}

        
		mouse.delta_x = 0.0f;
		mouse.delta_y = 0.0f;
    }

    //----------------------------------------------------------------------------
	//
	// Callbacks
	//
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		mouse.previous_x = mouse.position_x;
		mouse.previous_y = mouse.position_y;

		mouse.position_x = xpos;
		mouse.position_y = ypos;

		mouse.delta_x = mouse.position_x - mouse.previous_x;
		mouse.delta_y = mouse.position_y - mouse.previous_y;
	}

    void debugGUI()
    {
	    ImGui::Begin("Debug Info");

    	ImGui::Text("Hovering over UI: %d", mouse.hoveringOverUI);
    	ImGui::Separator();

	    ImGui::Text("Mouse position: %f %f", mouse.position_x, mouse.position_y);
		ImGui::Text("Mouse delta: %f %f", mouse.delta_x, mouse.delta_y);
		ImGui::Text("Mouse click point: %f %f", mouse.clickPoint_x, mouse.clickPoint_y);
		ImGui::Text("Mouse click vector: %f %f", mouse.clickVector_x, mouse.clickVector_y);
		ImGui::Separator();

        ImGui::End();
    }
};
