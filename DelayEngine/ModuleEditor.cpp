#include "ModuleEditor.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "SDL.h"
#include <GL/glew.h>
#include <IL/il.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/float3x4.h"

ModuleEditor::ModuleEditor()
{
    actualStatus = UPDATE_CONTINUE;
    enableWindows = true;
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
        if (ImGui::BeginMenu("Editor Windows"))
        {
            if (ImGui::MenuItem("Put Desktop Window")) {
                //The window size of the editor must be in relation to the desktop size (you can request the desktop screen size from SDL).
                SDL_DisplayMode DM;
                SDL_GetCurrentDisplayMode(0, &DM);
                App->window->width = DM.w;
                App->window->height = DM.h;
                SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
                SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL| SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            if (ImGui::MenuItem("Put Resizable Window")) {
                App->window->width = SCREEN_WIDTH;
                App->window->height = SCREEN_HEIGHT;
                SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
                SDL_SetWindowFullscreen(App->window->window, SDL_FALSE);
            }
            if (ImGui::MenuItem("Enable Info Windows")) {
                enableWindows = true;
            }
            if (ImGui::MenuItem("Disable Info Windows")) {
                enableWindows = false;
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
    if (enableWindows) {
        ImGui::Begin("Configuration", &enableWindows);
        //ImGui::Text("dear imgui says hello. (%s)", IMGUI_VERSION);
        if (ImGui::CollapsingHeader("Window"))
        {
            ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
            if (ImGui::TreeNode("Hardware Detection"))
            {
                ImGui::BulletText("Graphics Card: %s", glGetString(GL_RENDERER));
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Software Versions"))
            {
                ImGui::BulletText("OpenGL: %s", glGetString(GL_VERSION));
                SDL_version compiled;
                SDL_VERSION(&compiled);
                ImGui::BulletText("SDL: %d.%d.%d\n", compiled.major, compiled.minor, compiled.patch);
                ImGui::BulletText("DevIL: %d\n", IL_VERSION);
                ImGui::TreePop();
            }
            // Column header
            ImGui::Columns(3, NULL, false);
            ImGui::Text("    Width    "); ImGui::NextColumn();
            ImGui::Text("    Height   "); ImGui::NextColumn();
            ImGui::Text("         "); ImGui::NextColumn();
            ImGui::DragInt("", &App->window->width, 0.0f); ImGui::NextColumn();
            ImGui::DragInt("", &App->window->height, 0.0f); ImGui::NextColumn();
            ImGui::Text("Size"); ImGui::NextColumn();
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
            ImGui::Text("Position"); ImGui::NextColumn();
            float3 frontCamera = App->camera->getFront();
            ImGui::DragFloat("", &frontCamera.x, 0.0f); ImGui::NextColumn();
            ImGui::DragFloat("", &frontCamera.y, 0.0f); ImGui::NextColumn();
            ImGui::DragFloat("", &frontCamera.z, 0.0f); ImGui::NextColumn();
            ImGui::Text("Front"); ImGui::NextColumn();
            float3 upCamera = App->camera->getUp();
            ImGui::DragFloat("", &upCamera.x, 0.0f); ImGui::NextColumn();
            ImGui::DragFloat("", &upCamera.y, 0.0f); ImGui::NextColumn();
            ImGui::DragFloat("", &upCamera.z, 0.0f); ImGui::NextColumn();
            ImGui::Text("Up"); ImGui::NextColumn();
            ImGui::Separator();
            float fovCamera = App->camera->getFOV();
            ImGui::DragFloat("", &fovCamera, 0.0f); ImGui::NextColumn();
            ImGui::Text("FOV");
            ImGui::NextColumn();
            float aspectRadioCamera = App->camera->getAspectRadio();
            ImGui::DragFloat("", &aspectRadioCamera, 0.0f); ImGui::NextColumn();
            ImGui::Text("Aspect Radio");
            ImGui::NextColumn();
            float zNearCamera = App->camera->getZNear();
            ImGui::DragFloat("", &zNearCamera, 0.0f); ImGui::NextColumn();
            ImGui::Text("Near");
            ImGui::NextColumn();
            float zFarCamera = App->camera->getZFar();
            ImGui::DragFloat("", &zFarCamera, 0.0f); ImGui::NextColumn();
            ImGui::Text("Far");
            ImGui::NextColumn();
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
    
}

void ModuleEditor::setBottomMenu()
{
    if (enableWindows) {
        ImGui::Begin("BOTTOM", &enableWindows);
            focusedBottom = ImGui::IsWindowFocused();
        ImGui::End();
    }
}

void ModuleEditor:: setDockSpace()
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}