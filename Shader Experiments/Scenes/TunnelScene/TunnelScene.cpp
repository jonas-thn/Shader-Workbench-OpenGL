#include "TunnelScene.h"
#include "../../Application.h" 

TunnelScene::TunnelScene() : 
    tunnelShder("./Shader/Tunnel/tunnelShader.vert", "./Shader/Tunnel/tunnelShader.frag"),
    screen(std::vector<float>(std::begin(screenVertices), std::end(screenVertices)),{})
{
    meshList.push_back(&screen);
}

void TunnelScene::Update(float dt) {}

void TunnelScene::Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution)
{
    for (int i = 0; i < meshList.size(); i++)
    {
        meshList[i]->Draw(tunnelShder, view, projection, camPos, time, resolution, i, 0);
    }
}

void TunnelScene::OnActivate(IApplicationContext* cameraControl)
{
    cameraControl->SetCameraConfig(2.5f, 1.0f, 0.0f);
}

void TunnelScene::OnGuiRender() 
{
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Core Technique:");
    ImGui::BulletText("Fractal Raymarching");
    ImGui::BulletText("Infinite Repetition");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Visuals:");
    ImGui::BulletText("Sinewave Distortion");
    ImGui::BulletText("Distance-Based Fog");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
}