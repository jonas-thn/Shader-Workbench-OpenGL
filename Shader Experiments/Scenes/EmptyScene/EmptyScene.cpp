#include "EmptyScene.h"
#include "../../Application.h"

EmptyScene::EmptyScene() : 
    standardShader("./Shader/Standard/standardShader.vert", "./Shader/Standard/standardShader.frag"),
    text("./Models/text.obj")
{
    text.Rotate(-120, glm::vec3(0.0, 1.0, 0.0));
    meshList.push_back(&text);
}

void EmptyScene::Update(float dt) {}

void EmptyScene::Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time)
{
    for (int i = 0; i < meshList.size(); i++)
    {
        meshList[i]->Draw(standardShader, view, projection, camPos, time, i, 0);
    }
}

void EmptyScene::OnActivate(ICameraControl* cameraControl)
{
    cameraControl->SetCameraConfig(2.0f, 1.0f, 0.0f);
}

void EmptyScene::OnGuiRender() 
{
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Select a Scene!");
}