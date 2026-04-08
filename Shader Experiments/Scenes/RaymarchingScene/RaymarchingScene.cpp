#include "RaymarchingScene.h"
#include "../../Application.h" 

RaymarchingScene::RaymarchingScene() :
    raymarchShader("./Shader/Raymarching/raymarchShader.vert", "./Shader/Raymarching/raymarchShader.frag"),
    screen(std::vector<float>(std::begin(screenVertices), std::end(screenVertices)),{})
{
    meshList.push_back(&screen);
}

void RaymarchingScene::Update(float dt) {}

void RaymarchingScene::Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution)
{
    for (int i = 0; i < meshList.size(); i++)
    {
        meshList[i]->Draw(raymarchShader, view, projection, camPos, time, resolution, i, 0);
    }
}

void RaymarchingScene::OnActivate(IApplicationContext* cameraControl)
{
    cameraControl->SetCameraConfig(2.5f, 1.0f, 0.0f);
}

void RaymarchingScene::OnGuiRender()
{
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Core Technique:");
    ImGui::BulletText("Raymarching (Sphere Tracing)");
    ImGui::BulletText("Signed Distance Fields (SDF)");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Lighting & Atmosphere:");
    ImGui::BulletText("Soft Penumbra Shadows");
    ImGui::BulletText("SDF-based Ambient Occlusion");
    ImGui::BulletText("Exponential Depth Fog");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Modeling Logic:");
    ImGui::BulletText("Smooth Boolean Blending");
    ImGui::BulletText("Dynamic Color Interpolation");
}