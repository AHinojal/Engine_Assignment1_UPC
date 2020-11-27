#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleDebugDraw.h"
#include "ModuleTexture.h"
#include "SDL.h"
#include <GL/glew.h>
#include "Game/debug_draw/debugdraw.h"
#include "MathGeoLib/Math/float4x4.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	// OPENGL INITIALIZATION
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits

	// Creating Renderer CONTEXT
	LOG("Creating Renderer context");
	SDL_GLContext glcontext = SDL_GL_CreateContext(App->window->window);

	//glewInit();
	GLenum err = glewInit();
	if (GLEW_OK != err) printf("Error: %s\n", glewGetErrorString(err));
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//glEnable(GL_DEPTH_TEST); // Enable depth test
	//glEnable(GL_CULL_FACE); // Enable cull backward faces
	//glFrontFace(GL_CCW); // Front faces will be counter clockwise

	return true;
}

update_status ModuleRender::PreUpdate()
{
	// Setup Viewport
	glViewport(0,0, App->window->width, App->window->height);
	SDL_GetWindowSize(App->window->window, &App->window->width, &App->window->height);

	// Deleting color's buffer 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	// Put axis here -> not print triangle if you put it before glDrawArrays
	renderCoordinateAxis();
	App->debugDraw->Draw(App->camera->getViewMatrix(), App->camera->getProjectionMatrix(), App->window->width, App->window->height);

	// TODO: retrieve model view and projection -> Get it from the camera
	glUseProgram(App->program->GetProgram());
	glUniformMatrix4fv(glGetUniformLocation(App->program->GetProgram(), "model"), 1, GL_TRUE, &App->camera->getModelMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->GetProgram(), "view"), 1, GL_TRUE, &App->camera->getViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->GetProgram(), "proj"), 1, GL_TRUE, &App->camera->getProjectionMatrix()[0][0]);
	// TODO: bind buffer and vertex attributes
	// WORKING WITH VBO
	float vtx_data[] = {
		// first triangle
		-1.0f, -1.0f, 0.0f, // ← v0 pos
		1.0f, -1.0f, 0.0f, // ← v1 pos
		-1.0f, 1.0f, 0.0f, // ← v2 pos
		1.0f, 1.0f, 0.0f, // ← v3 pos

		0.0f, 0.0f, // ← v0 texcoord
		1.0f, .0f, // ← v1 texcoord
		0.0f, 1.0f, // ← v2 texcoord
		1.0f, 1.0f, // ← v2 texcoord


		/*Testing: Triangle
		-1.0f, -1.0f, 0.0f, // ← v0 pos
		1.0f, -1.0f, 0.0f, // ← v1 pos
		0.0f, 1.0f, 0.0f, // ← v2 pos
		0.0f, 0.0f, // ← v0 texcoord
		1.0f, 0.0f, // ← v1 texcoord
		0.5f, -1.0f, // ← v2 texcoord*/
	};

	// CREATE VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// CREATE VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
	unsigned int indexes[] = {  // note that we start from 0!
		0, 1, 2,   // first triangle
		2, 1, 3 // second triangle
	};

	// CREATE EBO
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 4 * 3)); // last parameter buffer offset
	glEnableVertexAttribArray(1);
	
	// Enable texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, App->texture->LoadTexture());
	glUniform1i(glGetUniformLocation(App->program->GetProgram(), "mytexture"), 0);
	
	// 1 triangle to draw = 3 vertices
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//// 2 triangle to draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{


 	// OpenGL Swap Frame Buffer
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	// Delete VBO, VAO & EBO
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &ebo);
	// OpenGL Destruction context
	SDL_GL_DeleteContext(context);

	LOG("Destroying renderer");

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
	LOG("Window Resized Render - W: %d, H: %d", width, height);
}

void ModuleRender::renderCoordinateAxis()
{
	// FPS CAMERA CLASS
	// Grid Engine
	// red X
	// green Y
	// blue Z
	dd::axisTriad(float4x4::identity, 0.1f, 1.0f);
	dd::xzSquareGrid(-10, 10, 0.0f, 1.0f, dd::colors::Gray);
}



