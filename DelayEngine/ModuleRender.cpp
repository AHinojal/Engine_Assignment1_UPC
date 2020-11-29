#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleDebugDraw.h"
#include "ModuleTexture.h"
#include "ModuleModel.h"
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

	// Load Baker House when initialize the application
	App->model->Load("..\\Game\\assets\\fbx\\BakerHouse.fbx");
	//App->model->Load("..\\Game\\assets\\WoodGirl.fbx");

	return true;
}

update_status ModuleRender::PreUpdate()
{
	// Setup Viewport
	glViewport(0, 0, App->window->width, App->window->height);
	// glViewport(App->window->width - App->window->width / 1.25, App->window->height - App->window->height / 1.25, App->window->width / 1.25, App->window->height / 1.25);
	SDL_GetWindowSize(App->window->window, &App->window->width, &App->window->height);

	// Deleting color's buffer 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	// Draw our grid
	App->debugDraw->Draw(App->camera->getViewMatrix(), App->camera->getProjectionMatrix(), App->window->width, App->window->height);

	// Load Lenna Texture
	/*Mesh m;
	m.DrawLenna();*/

	// Draw out model
	App->model->Draw();

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
	// OpenGL Destruction context
	SDL_GL_DeleteContext(context);

	LOG("Destroying renderer");

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
	LOG("Window Resized Render - W: %d, H: %d", width, height);
}



