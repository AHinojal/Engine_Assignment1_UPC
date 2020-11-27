#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "SDL.h"
#include <GL\glew.h>
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float3x3.h"
#include "MathGeoLib/Math/float4x4.h"
#include "MathGeoLib/Math/float3.h"

ModuleCamera::ModuleCamera()
{
	// FOV range [0-179]
	// Set by default in 75
	verticalFOV = 90;
	// Values Z
	zNear = 0.2f;
	zFar = 100.0f; //200.0f
	position = float3(0, 2, 6);
	standardSpeed = 0.25;
	actualSpeed = standardSpeed;
}

// Destructor
ModuleCamera::~ModuleCamera()
{
}

// Called before render is available
bool ModuleCamera::Init()
{
	// aspectRadio = width / height
	aspectRadio = (float)App->window->width / (float)App->window->height;
	LOG("Aspect Radio: %f", aspectRadio);
	// Options frustum put here to can update rotation
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(zNear, zFar);
	frustum.SetVerticalFovAndAspectRatio(DEGTORAD(verticalFOV), aspectRadio);
	//frustum.SetVerticalFovAndAspectRatio(verticalFOV, aspectRadio);
	// Move position camera
	frustum.SetPos(position);
	// Move camera forward and backward - vector (0,0,1)
	frustum.SetFront(-float3::unitZ);
	// Rotation camera - vector (0,1,0)
	frustum.SetUp(float3::unitY);

	return true;
}

update_status ModuleCamera::PreUpdate()
{
	projectionMatrix = frustum.ProjectionMatrix(); //<-- Important to transpose!
	viewMatrix = frustum.ViewMatrix(); //<-- Important to transpose!  
	//modelMatrix = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f), float4x4::RotateZ(pi / 4.0f), float3(2.0f, 1.0f, 0.0f));
	modelMatrix = float4x4::identity;

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update()
{
	//Send the frustum projection matrix to OpenGL
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*(projectionMatrix).v);

	//Send the frustum view matrix to OpenGL
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*(viewMatrix.Transposed()).v);

	// TRANSFORMATIONS WINDOW
	if (App->window->sizeChanged) {
		LOG("Camera detect resize window");
		setAspectRadio();
		LOG("Aspect Radio: %f", aspectRadio);
		LOG("Position Camera: %f %f %f", position.x, position.y, position.z);
		frustum.SetVerticalFovAndAspectRatio(DEGTORAD(verticalFOV), aspectRadio);
		//App->renderer->WindowResized(App->window->width, App->window->height);
		App->window->sizeChanged = false;
	}

	setFOVButtons();

	// TRANSFORMATIONS CAMERA
	deltaTime = clock() - oldTime;
	oldTime = clock();

	// Callbacks
	increaseCameraSpeed();
	// While Right clicking, “WASD” fps-like movement and free look around must be enabled.
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT)){
		goUpAndDown();
		zoomForwardAndBackward();
		App->input->stopScrollWheel(); // To control zoom with the mouse wheel
		moveLeftAndRight();
	}
	rotatePitch();
	rotateYaw();
	rotateRoll();
	rotatePitchAndYawWithMouse();

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleCamera::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window

	return true;
}

void ModuleCamera::setFOVButtons()
{
	if (App->input->GetKey(SDL_SCANCODE_F)) { // INCREASE FOV
		if (verticalFOV >= 0 && verticalFOV < 179) {
			verticalFOV += 1;
		}
		
	}
	if (App->input->GetKey(SDL_SCANCODE_H)) { // DECREASE FOV
		if (verticalFOV > 0 && verticalFOV < 180) {
			verticalFOV -= 1;
		}
	}
	frustum.SetVerticalFovAndAspectRatio(DEGTORAD(verticalFOV), aspectRadio);
}

void ModuleCamera::increaseCameraSpeed()
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		actualSpeed = standardSpeed * 3;
	} else {
		actualSpeed = standardSpeed;
	}
}

void ModuleCamera::goUpAndDown()
{
	if (App->input->GetKey(SDL_SCANCODE_Q)) { // UP
		position.y += actualSpeed;
		frustum.SetPos(position);
	}
	if (App->input->GetKey(SDL_SCANCODE_E)) { // DOWN
		position.y -= actualSpeed;
		frustum.SetPos(position);
	}
}

void ModuleCamera::zoomForwardAndBackward()
{
	//Mouse wheel should zoom in and out
	if (App->input->GetScrollWheel() > 0 || App->input->GetKey(SDL_SCANCODE_W)) { // FORWARD
		// Translate actual position * (vectorFront (0,0,1) * speed)
		frustum.Translate(frustum.Front() * actualSpeed);
			// Update new position with the translation
			position = frustum.Pos();
			frustum.SetPos(position);
	}
	if (App->input->GetScrollWheel() < 0 || App->input->GetKey(SDL_SCANCODE_S)) { // BACKWARD
		frustum.Translate(frustum.Front() * -actualSpeed);
		position = frustum.Pos();
		frustum.SetPos(position);
	}
}

void ModuleCamera::moveLeftAndRight()
{
	if (App->input->GetKey(SDL_SCANCODE_A)) { // LEFT
		// Translate actual position * (vectorFront (0,0,1) * speed)
		frustum.Translate(frustum.WorldRight() * -actualSpeed);
		// Update new position with the translation
		position = frustum.Pos();
		frustum.SetPos(position);
	}
	if (App->input->GetKey(SDL_SCANCODE_D)) { // RIGHT
		frustum.Translate(frustum.WorldRight() * actualSpeed);
		position = frustum.Pos();
		frustum.SetPos(position);
	}
}

void ModuleCamera::rotatePitch()
{
	if (App->input->GetKey(SDL_SCANCODE_UP)) { // LOOK UP
		float3 col1 = float3(1, 0 , 0);
		float3 col2 = float3(0, cos(actualSpeed), -sin(actualSpeed));
		float3 col3 = float3(0, sin(actualSpeed), cos(actualSpeed));
		float3x3 rotationMatrix;
		rotationMatrix.SetCol(0, col1);
		rotationMatrix.SetCol(1, col2);
		rotationMatrix.SetCol(2, col3);
		doRotation(rotationMatrix);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN)) { // LOOK DOWN
		float3 col1 = float3(1, 0, 0);
		float3 col2 = float3(0, cos(-actualSpeed), -sin(-actualSpeed));
		float3 col3 = float3(0, sin(-actualSpeed), cos(-actualSpeed));
		float3x3 rotationMatrix;
		rotationMatrix.SetCol(0, col1);
		rotationMatrix.SetCol(1, col2);
		rotationMatrix.SetCol(2, col3);
		doRotation(rotationMatrix);
	}
}

void ModuleCamera::rotateYaw()
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT)) { // LOOK LEFT
		float3 col1 = float3(cos(actualSpeed), 0, sin(actualSpeed));
		float3 col2 = float3(0, 1, 0);
		float3 col3 = float3(-sin(actualSpeed), 0, cos(actualSpeed));
		float3x3 rotationMatrix;
		rotationMatrix.SetCol(0, col1);
		rotationMatrix.SetCol(1, col2);
		rotationMatrix.SetCol(2, col3);
		doRotation(rotationMatrix);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT)) { // LOOK RIGHT
		float3 col1 = float3(cos(-actualSpeed), 0, sin(-actualSpeed));
		float3 col2 = float3(0, 1, 0);
		float3 col3 = float3(-sin(-actualSpeed), 0, cos(-actualSpeed));
		float3x3 rotationMatrix;
		rotationMatrix.SetCol(0, col1);
		rotationMatrix.SetCol(1, col2);
		rotationMatrix.SetCol(2, col3);
		doRotation(rotationMatrix);
	}
}

void ModuleCamera::rotateRoll()
{
	if (App->input->GetKey(SDL_SCANCODE_Z)) { // TURN LEFT
		float3 col1 = float3(cos(actualSpeed), -sin(actualSpeed), 0);
		float3 col2 = float3(sin(actualSpeed), cos(actualSpeed), 0);
		float3 col3 = float3(0, 0, 1);
		float3x3 rotationMatrix;
		rotationMatrix.SetCol(0, col1);
		rotationMatrix.SetCol(1, col2);
		rotationMatrix.SetCol(2, col3);
		doRotation(rotationMatrix);
	}
	if (App->input->GetKey(SDL_SCANCODE_C)) { // TURN RIGHT
		float3 col1 = float3(cos(-actualSpeed), -sin(-actualSpeed), 0);
		float3 col2 = float3(sin(-actualSpeed), cos(-actualSpeed), 0);
		float3 col3 = float3(0, 0, 1);
		float3x3 rotationMatrix;
		rotationMatrix.SetCol(0, col1);
		rotationMatrix.SetCol(1, col2);
		rotationMatrix.SetCol(2, col3);
		doRotation(rotationMatrix);
	}
}

void ModuleCamera::rotatePitchAndYawWithMouse()
{
	iPoint point = App->input->GetMouseMotion();
	float3x3 rotationMatrix;
	actualSpeed = 0.001;
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) && !App->editor->getFocused()) {
		// PITCH
		float3 col1 = float3(1, 0, 0);
		float3 col2 = float3(0, cos(point.y * actualSpeed * deltaTime), -sin(point.y * actualSpeed * deltaTime));
		float3 col3 = float3(0, sin(point.y * actualSpeed * deltaTime), cos(point.y * actualSpeed * deltaTime));
		rotationMatrix.SetCol(0, col1);
		rotationMatrix.SetCol(1, col2);
		rotationMatrix.SetCol(2, col3);
		doRotation(rotationMatrix);
		// YAW
		float3 col4 = float3(cos(point.x * actualSpeed * deltaTime), 0, sin(point.x * actualSpeed * deltaTime));
		float3 col5 = float3(0, 1, 0);
		float3 col6 = float3(-sin(point.x * actualSpeed * deltaTime), 0, cos(point.x * actualSpeed * deltaTime));
		rotationMatrix.SetCol(0, col4);
		rotationMatrix.SetCol(1, col5);
		rotationMatrix.SetCol(2, col6);
		doRotation(rotationMatrix);
	}
}

void ModuleCamera::doRotation(float3x3& rotationMatrix)
{
	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationMatrix.MulDir(oldFront));
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationMatrix.MulDir(oldUp));
}



