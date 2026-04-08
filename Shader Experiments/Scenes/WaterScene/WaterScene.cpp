#include "WaterScene.h"
#include "../../Application.h"

WaterScene::WaterScene() :
    waterShader("./Shader/Water/waterShader.vert", "./Shader/Water/waterShader.frag"),
    screen(std::vector<float>(std::begin(screenVertices), std::end(screenVertices)),{})
{
    meshList.push_back(&screen);
}

void WaterScene::Update(float dt) {}

void WaterScene::Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution)
{
    for (int i = 0; i < meshList.size(); i++)
    {
        meshList[i]->Draw(waterShader, view, projection, camPos, time, resolution, i, 0);
    }
}

void WaterScene::OnActivate(ICameraControl* cameraControl)
{
    cameraControl->SetCameraConfig(2.5f, 1.0f, 0.0f);
}

void WaterScene::OnGuiRender()
{
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Core Technique:");
    ImGui::BulletText("SDF Water Displacement");
    ImGui::BulletText("FBM Noise Animation");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Lighting Model:");
    ImGui::BulletText("Physical Specular (Blinn-Phong)");
    ImGui::BulletText("Dual-Layer Fresnel");
    ImGui::BulletText("Height-Based Foam/Scattering");
}