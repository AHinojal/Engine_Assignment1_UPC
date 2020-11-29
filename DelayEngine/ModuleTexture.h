#pragma once
#include "Module.h"
class ModuleTexture : public Module
{
public:

	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	unsigned LoadTexture(const char* fileData);
	int getWidth() { return width; };
	int getHeight() { return height; };

private:
	int width, height;
};

