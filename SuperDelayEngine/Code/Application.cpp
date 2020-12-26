#pragma once
#include "Application.h"
#include "./Modules/ModuleWindow.h"
#include "./Modules/ModuleRender.h"
#include "./Modules/ModuleInput.h"
#include "./Modules/ModuleCamera.h"
#include "./Modules/ModuleEditor.h"
#include "./Modules/ModuleProgram.h"
#include "./Modules/ModuleDebugDraw.h"
#include "./Modules/ModuleTexture.h"
#include "./Modules/ModuleModel.h"

#include "../../Libraries/Brofiler/Brofiler/Brofiler.h"

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(input = new ModuleInput());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(texture = new ModuleTexture());
	modules.push_back(model = new ModuleModel());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(debugDraw = new ModuleDebugDraw());
	// ModuleProgram is the last one - to load correctly the shaders
	modules.push_back(program = new ModuleProgram());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	BROFILER_CATEGORY("Pre-Update", Profiler::Color::Orchid);
	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	BROFILER_CATEGORY("Update", Profiler::Color::Orchid);
	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	BROFILER_CATEGORY("Post-Update", Profiler::Color::Orchid);
	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}
