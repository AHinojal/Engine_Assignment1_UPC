#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL.h"
#include <GL\glew.h>
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float3x3.h"
#include "MathGeoLib/Math/float3.h"

ModuleCamera::ModuleCamera()
{
	position = float3(0, 1, -2);
	actualSpeed = standardSpeed;
}

// Destructor
ModuleCamera::~ModuleCamera()
{
}

// Called before render is available
bool ModuleCamera::Init()
{
	return true;
}

update_status ModuleCamera::PreUpdate()
{
	// TODO: Put in a ModuleCamera all this stuff
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD(90), 1.3f);

	// Move position camera
	frustum.SetPos(position);
	// Move camera forward and backward - vector (0,0,1)
	frustum.SetFront(float3::unitZ);
	// Rotation camera - vector (0,1,0)
	frustum.SetUp(float3::unitY);

	projectionMatrix = frustum.ProjectionMatrix().Transposed(); //<-- Important to transpose!
	viewMatrix = frustum.ViewMatrix(); //<-- Important to transpose!  
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

	// FPS CAMERA CLASS
	// Grid Engine
	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();

	// Coordenate Axis
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	// red X
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	// green Y
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	// blue Z
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();
	glLineWidth(1.0f);

	// Color Grid
	glColor4f(1.0f, 1.0, 1.0f, 1.0f);

	// Callbacks
	increaseCameraSpeed();
	goUpAndDown();
	zoomForwardAndBackward();
	moveLeftAndRight();
	rotatePitch();

	App->input->stopScrollWheel(); // To control zoom with the mouse wheel

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
	if (App->input->GetKey(SDL_SCANCODE_UP)) { // FORWARD
		float3 col1 = float3(cos(actualSpeed), 0, sin(actualSpeed));
		float3 col2 = float3(0, 1, 0);
		float3 col3 = float3(-sin(actualSpeed), 0, cos(actualSpeed));
		float3x3 rotationMatrix;
		rotationMatrix.SetCol(0, col1);
		rotationMatrix.SetCol(1, col2);
		rotationMatrix.SetCol(2, col3);
		doRotation(rotationMatrix);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN)) { // BACKWARD
		//float3x3 rotationMatrix = 
		//doRotation(rotationMatrix);
	}
}

void ModuleCamera::doRotation(float3x3& rotationMatrix)
{
	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationMatrix.MulDir(oldFront));
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationMatrix.MulDir(oldUp));
}



