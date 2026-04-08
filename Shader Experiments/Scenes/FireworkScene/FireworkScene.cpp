#include "FireworkScene.h"
#include "../../Application.h" 

FireworkScene::FireworkScene() : 
    fireworkShader("./Shader/Firework/fireworkShader.vert", "./Shader/Firework/fireworkShader.frag"),
    screen(std::vector<float>(std::begin(screenVertices), std::end(screenVertices)),{})
{
    meshList.push_back(&screen);
}

void FireworkScene::Update(float dt) {}

void FireworkScene::Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution)
{
    for (int i = 0; i < meshList.size(); i++)
    {
        meshList[i]->Draw(fireworkShader, view, projection, camPos, time, resolution, i, 0);
    }
}

void FireworkScene::OnActivate(ICameraControl* cameraControl)
{
    cameraControl->SetCameraConfig(2.5f, 1.0f, 0.0f);
}

void FireworkScene::OnGuiRender()
{
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Visual Technique:");
    ImGui::BulletText("Procedural 2D Particles");
    ImGui::BulletText("Additive Blending");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Animation & Math:");
    ImGui::BulletText("Polar Coordinate Expansion");
    ImGui::BulletText("Time-Staggered Loops");
    ImGui::BulletText("Hash-based Randomness");
}