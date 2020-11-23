#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow()
{
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	sizeChanged = false;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		if (WINDOW_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		/* Dont work because not render scene
		Uint32 windowID = SDL_GetWindowID(window);

		while (true) {
			static SDL_Event event;

			while (SDL_PollEvent(&event) != 0)
			{
				switch (event.type)
				{
				case SDL_WINDOWEVENT:
					if (event.window.windowID == windowID) {
						switch (event.window.event)
						{
						case SDL_WINDOWEVENT_RESIZED:
							width = event.window.data1;
							height = event.window.data2;
							LOG("Deteccion risize pantalla Window");
							break;
						}
						break;
					}
				}
				break;
			}
		}*/
		

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}

		
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

