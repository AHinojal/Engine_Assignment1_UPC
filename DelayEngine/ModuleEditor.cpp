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
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/float3x4.h"

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
    setLeftMenu();
    setBottomMenu();

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
    ImGui::BeginMainMenuBar();  
        if (ImGui::BeginMenu("About"))
        {
            ImGui::MenuItem("Engine name:", TITLE);
            ImGui::MenuItem("Author:", "ALVARO HINOJAL BLANCO");
            if (ImGui::MenuItem("GitHub:", GITHUB_URL)) {
                visitGitHub();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Quit"))
        {

            actualStatus = UPDATE_STOP;
            ImGui::EndMenu();
        }
    ImGui::EndMainMenuBar();
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
    // Demonstrate the various window flags. Typically you would just use the default!
    static bool no_titlebar = false;
    static bool no_scrollbar = false;
    static bool no_menu = false;
    static bool no_move = false;
    static bool no_resize = false;
    static bool no_collapse = false;
    static bool no_close = false;
    static bool no_nav = false;
    static bool no_background = false;
    static bool no_bring_to_front = false;
    static bool no_docking = false;

    ImGui::Begin("INFO");
        //ImGui::Text("dear imgui says hello. (%s)", IMGUI_VERSION);
        if (ImGui::CollapsingHeader("Window"))
        {
            // Column header
            ImGui::Columns(3, NULL, false);
            ImGui::Text("    W    "); ImGui::NextColumn();
            ImGui::Text("    H    "); ImGui::NextColumn();
            ImGui::Text("         "); ImGui::NextColumn();
            static int width = App->window->width;
            static int height = App->window->height;
            ImGui::DragInt("", &width, 0.0f); ImGui::NextColumn();
            ImGui::DragInt("", &height, 0.0f); ImGui::NextColumn();
            ImGui::Text("Tamaño"); ImGui::NextColumn();
        }
        if (ImGui::CollapsingHeader("Camera"))
        {
            // Column header
            ImGui::Columns(4, NULL, false);
            ImGui::Text("    x    "); ImGui::NextColumn();
            ImGui::Text("    y    "); ImGui::NextColumn();
            ImGui::Text("    z    "); ImGui::NextColumn();
            ImGui::Text("         "); ImGui::NextColumn();
            float3 positionCamera = App->camera->getPosition();
            ImGui::DragFloat("", &positionCamera.x, 0.0f); ImGui::NextColumn();
            ImGui::DragFloat("", &positionCamera.y, 0.0f); ImGui::NextColumn();
            ImGui::DragFloat("", &positionCamera.z, 0.0f); ImGui::NextColumn();
            ImGui::Text("Posicion"); ImGui::NextColumn();
        }

        if (ImGui::CollapsingHeader("Configuration"))
        {

        }

        if (ImGui::CollapsingHeader("Window options"))
        {
            //ImGui::Checkbox("No titlebar", &no_titlebar); ImGui::SameLine(150);
        }

        // End of ShowDemoWindow()
        focusedLeft = ImGui::IsWindowFocused();
    ImGui::End();
}

void ModuleEditor::setBottomMenu()
{
    ImGui::Begin("BOTTOM");
        focusedBottom = ImGui::IsWindowFocused();
    ImGui::End();
}

void ModuleEditor:: setDockSpace()
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}