#include "Application.h"

#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "Scenes/Scene.h"

#include "Scenes/LightingScene/LightingScene.h"
#include "Scenes/EmptyScene/EmptyScene.h"
#include "Scenes/OtherScene/OtherScene.h"
#include "Scenes/PlanetScene/PlanetScene.h"
#include "Scenes/RaymarchingScene/RaymarchingScene.h"
#include "Scenes/TerrainScene/TerrainScene.h"
#include "Scenes/GrassScene/GrassScene.h"
#include "Scenes/FireworkScene/FireworkScene.h"
#include "Scenes/WaterScene/WaterScene.h"
#include "Scenes/TunnelScene/TunnelScene.h"
#include "Scenes/CustomScene/CustomScene.h"

Application::Application()
{
    window = std::make_unique<GLWindow>("OpenGL Shader Editor", width, height);
    window->SetVSync(true);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        fprintf(stderr, "GLEW Init Error: %s\n", glewGetErrorString(err));
        running = false;
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplSDL2_InitForOpenGL(window->GetWindow(), window->GetContext());
    ImGui_ImplOpenGL3_Init("#version 130");

    InitImGuiStyle();

    glEnable(GL_DEPTH_TEST);
}

Application::~Application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Application::Setup()
{
    uiWidth = static_cast<int>(width * uiWidthPercent);
    int startViewportWidth = width - uiWidth;
    InitFBO(startViewportWidth, height);

    projection = glm::perspective(glm::radians(45.0f), (float)startViewportWidth / (float)height, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    sceneList.push_back(std::make_shared<EmptyScene>());
    sceneList.push_back(std::make_shared<LightingScene>());
    sceneList.push_back(std::make_shared<WaterScene>());
    sceneList.push_back(std::make_shared<RaymarchingScene>());
    sceneList.push_back(std::make_shared<GrassScene>());
    sceneList.push_back(std::make_shared<FireworkScene>());
    sceneList.push_back(std::make_shared<TunnelScene>());
    sceneList.push_back(std::make_shared<PlanetScene>());
    sceneList.push_back(std::make_shared<TerrainScene>());
    sceneList.push_back(std::make_shared<OtherScene>());
    sceneList.push_back(std::make_shared<CustomScene>());

    if (!sceneList.empty())
    {
        activeScene = sceneList[0].get();
        activeScene->OnActivate(this);
    }
}

void Application::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
        {
            running = false;
        }

        if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                width = event.window.data1;
                height = event.window.data2;
                uiWidth = static_cast<int>(width * uiWidthPercent);
            }
        }
    }
}

void Application::Update()
{
    float currentFrame = static_cast<float>(SDL_GetTicks());
    float deltaTime = (currentFrame - lastFrame) / 1000.0f;
    lastFrame = currentFrame;

    uiWidth = static_cast<int>(width * uiWidthPercent);

    static float angle = 0.0f;
    angle -= camSpeed * deltaTime;

    camPos.x = sin(angle) * camRadius;
    camPos.y = cameraHeight;
    camPos.z = cos(angle) * camRadius;

    view = glm::lookAt(camPos,
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(0.0f, 1.0f, 0.0f) 
    );

    if (activeScene)
    {
        activeScene->Update(deltaTime);
    }
}

void Application::Render()
{
    //pass 1
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, currentFboWidth, currentFboHeight);
    DrawScene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //pass 2
	glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
    DrawGUI();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    window->SwapBuffers();
}

void Application::DrawGUI()
{
    //settings
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2((float)(width - uiWidth), 0.0f));
    ImGui::SetNextWindowSize(ImVec2((float)uiWidth, (float)height));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 8.0f));

    ImGui::Begin("Shader Editor", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::SeparatorText("Select Scene");

    if (ImGui::BeginListBox("##SceneList", ImVec2(-1, 189))) 
    {
        for (int i = 0; i < sceneList.size(); i++)
        {
            Scene* s = sceneList[i].get();
            bool isSelected = (activeScene == s);

            if (ImGui::Selectable(s->GetName().c_str(), isSelected))
            {
                activeScene = s;
                activeScene->OnActivate(this);
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }

    ImGui::Separator();
    ImGui::Spacing();

    if (activeScene)
    {
        ImGui::SeparatorText(activeScene->GetName().c_str());
        ImGui::Spacing();
    
        activeScene->OnGuiRender();
    }

    ImGui::End();
    ImGui::PopStyleVar();

    //main window
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2((float)(width - uiWidth), (float)height));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("Scene Viewport", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    if (viewportPanelSize.x != currentFboWidth || viewportPanelSize.y != currentFboHeight)
    {
        ResizeFBO(static_cast<int>(viewportPanelSize.x), static_cast<int>(viewportPanelSize.y));
    }

    ImGui::Image((void*)(intptr_t)sceneTexture,ImVec2(currentFboWidth, currentFboHeight),ImVec2(0, 1),ImVec2(1, 0));

    ImGui::End();
    ImGui::PopStyleVar(2);

    ImGui::Render();
}

void Application::DrawScene()
{
    glClearColor(0.1f, 0.1f, 0.105f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (activeScene)
    {
        activeScene->Draw(view, projection, camPos, (SDL_GetTicks() * 0.001f));
    }
}

void Application::SetCameraConfig(float radius, float speed, float height)
{
    this->camRadius = radius;
    this->camSpeed = speed;
    this->cameraHeight = height;
}

void Application::InitImGuiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsDark();

    ImVec4 blackColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    ImVec4 grey02Color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    ImVec4 grey04Color = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    ImVec4 grey06Color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
    ImVec4 grey08Color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    ImVec4 whiteColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    style.Colors[ImGuiCol_FrameBg] = grey02Color;
    style.Colors[ImGuiCol_FrameBgHovered] = grey04Color;
    style.Colors[ImGuiCol_FrameBgActive] = grey06Color;
    style.Colors[ImGuiCol_PopupBg] = grey02Color;
    style.Colors[ImGuiCol_Border] = grey04Color;
    style.Colors[ImGuiCol_BorderShadow] = blackColor;
    style.Colors[ImGuiCol_Header] = grey04Color;
    style.Colors[ImGuiCol_HeaderHovered] = grey06Color;
    style.Colors[ImGuiCol_HeaderActive] = grey08Color;
    style.Colors[ImGuiCol_Text] = whiteColor;
    style.Colors[ImGuiCol_CheckMark] = whiteColor;
    style.Colors[ImGuiCol_ScrollbarBg] = grey02Color;
    style.Colors[ImGuiCol_ScrollbarGrab] = grey04Color;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = grey06Color;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = grey08Color;
    style.Colors[ImGuiCol_Button] = grey04Color;
    style.Colors[ImGuiCol_SliderGrab] = grey08Color;
    style.Colors[ImGuiCol_SliderGrabActive] = whiteColor;
    style.Colors[ImGuiCol_ButtonHovered] = grey06Color;
    style.Colors[ImGuiCol_ButtonActive] = grey08Color;
    style.Colors[ImGuiCol_TextSelectedBg] = grey06Color;
    style.Colors[ImGuiCol_Separator] = grey04Color;
    style.Colors[ImGuiCol_SeparatorHovered] = grey06Color;
    style.Colors[ImGuiCol_SeparatorActive] = grey08Color;
    style.Colors[ImGuiCol_TitleBg] = grey02Color;
    style.Colors[ImGuiCol_TitleBgActive] = grey04Color;
    style.Colors[ImGuiCol_TitleBgCollapsed] = grey02Color;
    style.Colors[ImGuiCol_ResizeGrip] = grey04Color;
    style.Colors[ImGuiCol_ResizeGripHovered] = grey06Color;
    style.Colors[ImGuiCol_ResizeGripActive] = grey08Color;
    style.Colors[ImGuiCol_DragDropTarget] = grey08Color;
    style.Colors[ImGuiCol_NavHighlight] = grey06Color;
    style.Colors[ImGuiCol_NavWindowingHighlight] = grey06Color;
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0, 0, 0, 0.4f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.4f);
    style.WindowPadding = ImVec2(20.0f, 20.0f);
}

void Application::InitFBO(int width, int height)
{
	currentFboWidth = width;
	currentFboHeight = height;

	glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &sceneTexture);
	glBindTexture(GL_TEXTURE_2D, sceneTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, currentFboWidth, currentFboHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTexture, 0);

    glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, currentFboWidth, currentFboHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "FBO Error: Framebuffer is not complete\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Application::ResizeFBO(int newWidth, int newHeight)
{
    if (newWidth <= 0 || newHeight <= 0)
    {
        return;
    }

    if (newWidth == currentFboWidth && newHeight == currentFboHeight)
    {
        return;
	}

	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &sceneTexture);
	glDeleteRenderbuffers(1, &rbo);

	InitFBO(newWidth, newHeight);

	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
}
