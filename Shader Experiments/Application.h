#pragma once

#include <gl/glew.h>

#include "ImGui/imgui.h"

#include <SDL.h>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLWindow.h"
#include "ICameraControl.h"

class Scene;

class Application : public ICameraControl
{
public:
    Application();
    ~Application();

    void Setup();
    void ProcessInput();
    void Update();
    void Render();

    void SetCameraConfig(float radius, float speed, float height) override;

public:
    bool running = true;

private:
    void DrawGUI();
    void DrawScene();

    void InitImGuiStyle();

    void InitFBO(int width, int height);
	void ResizeFBO(int newWidth, int newHeight);

private:
	std::unique_ptr<GLWindow> window;

    int width = 800;
    int height = 600;

    float lastFrame = 0.0f;

    Scene* activeScene = nullptr;
    std::vector<std::shared_ptr<Scene>> sceneList;

    glm::vec3 camPos;
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    float camRadius = 2.5f;
    float camSpeed = 1.0f;
    float cameraHeight = 0.0f;

    float uiWidthPercent = 0.35f;
    int uiWidth = 0;

    unsigned int fbo = 0;
	unsigned int sceneTexture = 0;
    unsigned int rbo = 0;

	int currentFboWidth = 0;
	int currentFboHeight = 0;
};