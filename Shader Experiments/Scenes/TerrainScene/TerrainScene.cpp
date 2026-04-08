#include "TerrainScene.h"
#include "../../Application.h"

TerrainScene::TerrainScene() :
    terrainShader("./Shader/Terrain/terrainShader.vert", "./Shader/Terrain/terrainShader.frag"),
    screen(std::vector<float>(std::begin(screenVertices), std::end(screenVertices)),{})
{
    meshList.push_back(&screen);
}

void TerrainScene::Update(float dt) {}

void TerrainScene::Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution)
{
    for (int i = 0; i < meshList.size(); i++)
    {
        meshList[i]->Draw(terrainShader, view, projection, camPos, time, resolution, i, 0);
    }
}

void TerrainScene::OnActivate(ICameraControl* cameraControl)
{
    cameraControl->SetCameraConfig(2.5f, 1.0f, 0.0f);
}

void TerrainScene::OnGuiRender()
{
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Core Technique:");
    ImGui::BulletText("Terrain Raymarching");
    ImGui::BulletText("Water Plane Union");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Atmosphere:");
    ImGui::BulletText("Distance Fog with Inscattering");
    ImGui::BulletText("Sun Glow & Sky Gradient");
    ImGui::BulletText("Hard Shadow Raycasting");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Generation:");
    ImGui::BulletText("Height-based Color Mixing");
    ImGui::BulletText("Fractal Brownian Motion (FBM)");
}