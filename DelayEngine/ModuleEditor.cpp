#include "ModuleEditor.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include <GL\glew.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"

ModuleEditor::ModuleEditor()
{
    actualStatus = UPDATE_CONTINUE;
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

    // Setup Dear ImGui style
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->getContext());
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

  	return true;
}

update_status ModuleEditor::PreUpdate()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
    showMainMenuBar();
    //setDockSpace();
    //setScene();
    //setLeftMenu();
    checkWindowHovered();


    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return actualStatus;
}

update_status ModuleEditor::PostUpdate()
{

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
	return true;
}

void ModuleEditor::showMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("GitHub"))
        {
            visitGitHub();
            //ImGui::EndMenu();
        }        
        if (ImGui::BeginMenu("About"))
        {
            ImGui::MenuItem("Engine name:", TITLE);
            ImGui::MenuItem("Author:", "ALVARO HINOJAL BLANCO");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Quit"))
        {

            actualStatus = UPDATE_STOP;
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ModuleEditor::setScene()
{
    ImGui::Begin("Scene Window");

    ImVec2 pos = ImGui::GetCursorScreenPos();

    //Give us the texture but not the rendering (App->camera,window,renderer->Update())
    /*ImGui::GetWindowDrawList()->AddImage(
        (void*)App->window->Update(), ImVec2(ImGui::GetCursorScreenPos()),
        ImVec2(ImGui::GetCursorScreenPos().x + App->window->width / 2, ImGui::GetCursorScreenPos().y + App->window->height / 2), ImVec2(0, 1), ImVec2(1, 0));*/

    ImGui::End();
}

void ModuleEditor::setLeftMenu()
{
    ImGui::Begin("LEFT");
    checkWindowHovered();

    ImGui::End();
}

void ModuleEditor:: setDockSpace()
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}