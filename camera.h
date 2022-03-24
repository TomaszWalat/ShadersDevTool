#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp> //includes GLM


class Camera 
{
private:
	// projection data
	GLfloat fov;
	GLfloat aspectRatioX;
	GLfloat aspectRatioY;
	GLfloat aspectRatio;
	GLfloat nearPlane; // Clipping plane
	GLfloat farPlane;  // Clipping plane

	// mvp
	glm::mat4 model;		// World
	glm::mat4 view;			// Camera
	glm::mat4 projection;	// Clip space

	// arcball data
	GLfloat movementSpeed;
	GLfloat rotationSpeed;
	GLfloat rotationRadius;				// Camera's distance from origin - inverted for arcball movement
	GLfloat horizontalRotationCap;		// Rotation cap around the screen's Y (Up) axis from identity matrix in radians
	GLfloat verticalRotationCap;		// Rotation cap around the screen's X (Right) axis from identity matrix in radians
	GLfloat currentHorizontalRotation;	// phi
	GLfloat currentVerticalRotation;	// theta
	glm::mat4 viewInverse;				// Camera axis that are always aligned with the sceeen


public:
	Camera();
	~Camera();

	// Movement in relation to self (i.e. screen plane)
	void moveRight();		// +X
	void moveLeft();		// -X
	void moveUp();			// +Y
	void moveDown();		// +Y
	void moveBackward();	// +Z
	void moveForward();		// -Z

	// Rotation in relation to self (i.e. screen plane)
	void rotateAroundTarget(glm::vec2 rotationDirection);
	void tilt(GLfloat direction);

	// Reset functions
	void resetCamera();
	void resetModel();
	void resetView();
	void resetProjection();

	// Update functions
	void updateInverse();
	void updateShadersMVP(GLuint shader);

	glm::mat4 getModel();
	glm::mat4 getView();
	glm::mat4 getProjection();

	// Draw functions
	// This function injects Dear ImGui elements into a UI panel
	// only call it inside of ImGui::Begin()-ImGui::End() code blocks!!!
	void drawGUI();

};

