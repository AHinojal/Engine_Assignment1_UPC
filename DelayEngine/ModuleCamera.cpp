#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include <GL\glew.h>

#include "MathGeoLib/Geometry/Frustum.h"

ModuleCamera::ModuleCamera()
{
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
	Frustum frustum;

	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD(90), 1.3f);

	//Send the frustum projection matrix to OpenGL
	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); //<-- Important to transpose!
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*projectionGL.v);

	frustum.SetPos(float3(0, 1, -2));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	float4x4 viewMatrix = frustum.ViewMatrix(); //<-- Important to transpose!
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*(viewMatrix.Transposed()).v);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update()
{

	// FPS CAMERA CLASS
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

	glColor4f(1.0f, 1.0, 1.0f, 1.0f);

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
