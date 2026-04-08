#include "PlanetScene.h"
#include "../../Application.h" 

PlanetScene::PlanetScene() :
    planetShader("./Shader/Planet/planetShader.vert", "./Shader/Planet/planetShader.frag"),
    screen(std::vector<float>(std::begin(screenVertices), std::end(screenVertices)), {})
{
    meshList.push_back(&screen);
}

void PlanetScene::Update(float dt) {}

void PlanetScene::Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution)
{
    for (int i = 0; i < meshList.size(); i++)
    {
        meshList[i]->Draw(planetShader, view, projection, camPos, time, resolution, i, 0);
    }
}

void PlanetScene::OnActivate(IApplicationContext* cameraControl)
{
    cameraControl->SetCameraConfig(2.5f, 1.0f, 0.0f);
}

void PlanetScene::OnGuiRender()
{
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Generation:");
    ImGui::BulletText("Procedural 3D FBM Noise Terrain");
    ImGui::BulletText("Bi-Planar Mapping & Moisture Mask");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Lighting & Effects:");
    ImGui::BulletText("Analytic Sphere Tracing");
    ImGui::BulletText("SDF Atmosphere Glow");
    ImGui::BulletText("Specular Water Masking");
}