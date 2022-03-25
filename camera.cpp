#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"


// -------- De/Constructors -------- //
Camera::Camera()
{
	fov = 45.0f;
	aspectRatioX = 16.0f;
	aspectRatioY = 9.0f;
	aspectRatio = aspectRatioX / aspectRatioY;
	nearPlane = 0.1f;
	farPlane = 40.0f;

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.5f, 0.0f));
	view = glm::mat4(1.0f);
	projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

	movementSpeed = 0.05f;
	rotationSpeed = 0.01f;
	rotationRadius = 10.0f; 
	horizontalRotationCap = 3.14159f;	//  pi  ==  half circle   == 180 degrees
	verticalRotationCap = 1.57079f;		// pi/2 == quarter circle ==  90 degrees
	currentHorizontalRotation = 0.0f;
	currentVerticalRotation = 0.0f;
	viewInverse = glm::mat4(1.0f);

	// Invert camera through origin for arcball style movement
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -rotationRadius));
}

Camera::~Camera()
{

}
// -------- End of de/constructors -------- //



// -------- Movement in relation to self (i.e. screen plane) -------- //
void Camera::moveRight()		// +X
{
	glm::vec3 moveVector = glm::vec3(-viewInverse[0].x, -viewInverse[0].y, -viewInverse[0].z) * movementSpeed;
	
	model = glm::translate(model, moveVector);
}

void Camera::moveLeft()		// -X
{
	glm::vec3 moveVector = glm::vec3(viewInverse[0].x, viewInverse[0].y, viewInverse[0].z) * movementSpeed;
	
	model = glm::translate(model, moveVector);
}

void Camera::moveUp()			// +Y
{
	glm::vec3 moveVector = glm::vec3(-viewInverse[1].x, -viewInverse[1].y, -viewInverse[1].z) * movementSpeed;
	
	model = glm::translate(model, moveVector);
}

void Camera::moveDown()		// +Y
{
	glm::vec3 moveVector = glm::vec3(viewInverse[1].x, viewInverse[1].y, viewInverse[1].z) * movementSpeed;
	
	model = glm::translate(model, moveVector);
}

void Camera::moveBackward()	// +Z
{
	glm::vec3 moveVector = glm::vec3(-viewInverse[2].x, -viewInverse[2].y, -viewInverse[2].z) * movementSpeed;
	
	model = glm::translate(model, moveVector);
}

void Camera::moveForward()	// -Z
{
	glm::vec3 moveVector = glm::vec3(viewInverse[2].x, viewInverse[2].y, viewInverse[2].z) * movementSpeed;
	
	model = glm::translate(model, moveVector);
}
// -------- End of movement fuctions -------- //



// -------- Rotation in relation to screen plane -------- //
void Camera::rotateAroundTarget(glm::vec2 rotationDirection)
{
	if (rotationDirection.x != 0)
	{
		GLfloat horizontalAmount = rotationDirection.x * rotationSpeed;
		GLfloat newHorizontal = currentHorizontalRotation + horizontalAmount;

		if (-horizontalRotationCap < newHorizontal && newHorizontal < horizontalRotationCap)
		{
			currentHorizontalRotation += horizontalAmount;
			
			view = glm::rotate(view, horizontalAmount, glm::vec3(viewInverse[1].x, viewInverse[1].y, viewInverse[1].z));
			
			viewInverse[0] = glm::vec4(view[0].x, view[1].x, view[2].x, view[3].x); // Update just the right direction
		}
	}

	if (rotationDirection.y != 0)
	{
		GLfloat verticalAmount = rotationDirection.y * rotationSpeed;
		GLfloat newVertical = currentVerticalRotation + verticalAmount;

		if (-verticalRotationCap < newVertical && newVertical < verticalRotationCap)
		{
			currentVerticalRotation += verticalAmount;

			view = glm::rotate(view, verticalAmount, glm::vec3(viewInverse[0].x, viewInverse[0].y, viewInverse[0].z));
		}
	}
	//updateInverse();
}

void Camera::tilt(GLfloat amount)
{
	view = glm::rotate(view, amount * rotationSpeed, glm::vec3(viewInverse[2].x, viewInverse[2].y, viewInverse[2].z));

	updateInverse();
}
// -------- End of rotation functions -------- //



// -------- Reset camera functions -------- //
void Camera::resetCamera()
{
	resetModel();
	resetView();
	resetProjection();
}

void Camera::resetModel()
{
	currentHorizontalRotation = 0.0f;
	currentVerticalRotation = 0.0f;
	
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.5f, 0.0f));
}

void Camera::resetView()
{
	view = glm::mat4(1.0f);
	viewInverse = glm::mat4(1.0f);

	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -rotationRadius)); // Invert camera through origin
}

void Camera::resetProjection()
{
	projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}
// -------- End of reset camera functions -------- //



// -------- Update functions -------- //
void Camera::updateInverse()
{
	viewInverse = glm::transpose(view);

	// Reset rotation trackers - "moves" the rotations caps
 	currentHorizontalRotation = 0.0f;
	currentVerticalRotation = 0.0f;
}

void Camera::updateShadersMVP(GLuint shader)
{
	// --- Update MVP uniforms in shader --- //
	int mvpLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(model));

	mvpLoc = glGetUniformLocation(shader, "view");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(view));

	mvpLoc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
// -------- End of update functions -------- //

// -------- Get functions -------- //
glm::mat4 Camera::getModel()
{
	return model;
}
glm::mat4 Camera::getView()
{
	return view;
}
glm::mat4 Camera::getProjection()
{
	return projection;
}

// -------- Draw/Display functions -------- //
// This function injects Dear ImGui elements into a UI panel
// only call it inside of ImGui::Begin()-ImGui::End() code blocks!!!
void Camera::drawGUI()
{
	if (ImGui::CollapsingHeader("Model:"))
	{
		ImGui::DragFloat4("Right##m", glm::value_ptr(model[0]), 0.01f);
		ImGui::DragFloat4("Up##m", glm::value_ptr(model[1]), 0.01f);
		ImGui::DragFloat4("Forward##m", glm::value_ptr(model[2]), 0.01f);
		ImGui::DragFloat4("Position##m", glm::value_ptr(model[3]), 0.01f);
		if (ImGui::Button("Reset##m"))
		{
			resetModel();
		}
	}
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("View:"))
	{
		ImGui::DragFloat4("Right##v", glm::value_ptr(view[0]), 0.01f);
		ImGui::DragFloat4("Up##v", glm::value_ptr(view[1]), 0.01f);
		ImGui::DragFloat4("Forward##v", glm::value_ptr(view[2]), 0.01f);
		ImGui::DragFloat4("Position##v", glm::value_ptr(view[3]), 0.01f);
		ImGui::Separator();
		if (ImGui::Button("Reset##v"))
		{
			resetView();
		}
	}
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Projection:"))
	{
		ImGui::DragFloat4("Right##p", glm::value_ptr(projection[0]), 0.01f);
		ImGui::DragFloat4("Up##p", glm::value_ptr(projection[1]), 0.01f);
		ImGui::DragFloat4("Forward##p", glm::value_ptr(projection[2]), 0.01f);
		ImGui::DragFloat4("Position##p", glm::value_ptr(projection[3]), 0.01f);
		ImGui::Separator();
		if (ImGui::Button("Reset##p"))
		{
			resetProjection();
		}
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::CollapsingHeader("Camera Debug Info"))
	{
		ImGui::Indent();
		if (ImGui::CollapsingHeader("Inverse View:"))
		{
			ImGui::DragFloat4("Right##iv", glm::value_ptr(viewInverse[0]), 0.01f);
			ImGui::DragFloat4("Up##iv", glm::value_ptr(viewInverse[1]), 0.01f);
			ImGui::DragFloat4("Forward##iv", glm::value_ptr(viewInverse[2]), 0.01f);
			ImGui::DragFloat4("Position##iv", glm::value_ptr(viewInverse[3]), 0.01f);
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Text("Horizontal angle (phi): %f ", currentHorizontalRotation);
		ImGui::Text("Vertical angle (theta): %f ", currentVerticalRotation);
		ImGui::Unindent();
		ImGui::Spacing();
	}
	ImGui::Separator();
	if (ImGui::Button("Reset Camera##mvp"))
	{
		resetCamera();
	}
}
// -------- End of draw/display functions -------- //