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
	App->model->Load(BAKER_HOUSE_PATH);

	return true;
}

update_status ModuleRender::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	// Framebuffer to do a scene window
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// create a color attachment texture
	glGenTextures(1, &textureRender);
	glBindTexture(GL_TEXTURE_2D, textureRender);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->window->width, App->window->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureRender, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, App->window->width, App->window->height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	// Setup Viewport
	glViewport(0, 0, App->window->width, App->window->height);
	// glViewport(App->window->width - App->window->width / 1.25, App->window->height - App->window->height / 1.25, App->window->width / 1.25, App->window->height / 1.25);
	SDL_GetWindowSize(App->window->window, &App->window->width, &App->window->height);

	// Deleting color's buffer 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw our grid
	App->debugDraw->Draw(App->camera->getViewMatrix(), App->camera->getProjectionMatrix(), App->window->width, App->window->height);

	// Load Lenna Texture
	/*Mesh m;
	m.DrawLenna();*/

	// Draw out model
	App->model->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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



