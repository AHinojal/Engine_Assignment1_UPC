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
	speed = 0.25;
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
	// Move camera forward and backward
	frustum.SetFront(float3::unitZ);
	// Rotation camera
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

	// CallMethods
	goUpAndDown();
	moveForwardAndBackward();

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

void ModuleCamera::goUpAndDown()
{
	if (App->input->GetKey(SDL_SCANCODE_Q)) {
		position.y += speed;
		frustum.SetPos(position);
	}
	if (App->input->GetKey(SDL_SCANCODE_E)) {
		position.y -= speed;
		frustum.SetPos(position);
	}
}

void ModuleCamera::moveForwardAndBackward()
{
	if (App->input->GetKey(SDL_SCANCODE_W)) {
		// Translate actual position * (vectorFront (0,0,1) * speed)
		frustum.Translate(frustum.Front() * speed);
		// Update new position with the translation
		position = frustum.Pos();
		frustum.SetPos(position);
	}
	if (App->input->GetKey(SDL_SCANCODE_S)) {
		frustum.Translate(frustum.Front() * -speed);
		position = frustum.Pos();
		frustum.SetPos(position);
	}
}


/*float3x3 rotationMatrix;
vec oldFront = frustum.Front().Normalized();
frustum.SetFront(rotationMatrix.MulDir(oldFront));
vec oldUp = frustum.Up().Normalized();
frustum.SetUp(rotationMatrix.MulDir(oldUp));*/
