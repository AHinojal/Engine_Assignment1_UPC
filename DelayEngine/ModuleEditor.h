#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	bool getFocused() { return focusedLeft || focusedBottom; };

private:
	bool focusedLeft, focusedBottom;
	void showMainMenuBar();
	void setScene();
	void setLeftMenu();
	void setBottomMenu();
	void setDockSpace();
	update_status actualStatus;
	void visitGitHub() {
		ShellExecute(NULL, "open", "https://github.com/AHinojal/Engine_Assignment1_UPC", NULL, NULL, SW_SHOWNORMAL);
	}
};

