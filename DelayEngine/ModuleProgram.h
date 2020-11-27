#pragma once
#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void CreateProgram();
	char* LoadShaderSource(const char* shader_file_name);
	unsigned CompileShader(unsigned type, const char* source);
	unsigned GetProgram() { return programId; }

private:
	unsigned vertexShader, fragmentShader;
	unsigned programId;
};

