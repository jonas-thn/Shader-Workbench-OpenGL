#include "OtherScene.h"
#include "../../Application.h" 

OtherScene::OtherScene() :
    noiseShader("./Shader/Noise/noiseShader.vert", "./Shader/Noise/noiseShader.frag"),
    rippleShader("./Shader/Lighting/lightingShader.vert", "./Shader/Lighting/lightingShader.frag"),
    sphereNoise("./Models/sphere_high.obj"),
    sphereRipple("./Models/sphere_high.obj"),
    monkeyExplode("./Models/monkey_high.obj"),
    monkeySphere("./Models/monkey_high.obj")
{
    sphereNoise.Translate(glm::vec3(1.5, 0.0, 0.0));
    meshList.push_back(&sphereNoise);

    sphereRipple.Translate(glm::vec3(-1.5, 0.0, 0.0));
    meshList.push_back(&sphereRipple);

    monkeyExplode.Translate(glm::vec3(0.0, 0.0, 1.5));
    monkeyExplode.SetScale(glm::vec3(0.6));
    meshList.push_back(&monkeyExplode);

    monkeySphere.Translate(glm::vec3(0.0, 0.0, -1.5));
    monkeySphere.Rotate(180, glm::vec3(0.0, 1.0, 0.0));
    monkeySphere.SetScale(glm::vec3(0.6));
    meshList.push_back(&monkeySphere);
}

void OtherScene::Update(float dt) {}

void OtherScene::Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution)
{
    sphereNoise.Draw(noiseShader, view, projection, camPos, time, resolution, 0, 0);
    sphereRipple.Draw(rippleShader, view, projection, camPos, time, resolution, 999, 0);
    monkeyExplode.Draw(rippleShader, view, projection, camPos, time, resolution, 998, 0);
    monkeySphere.Draw(rippleShader, view, projection, camPos, time, resolution, 997, 0);
}

void OtherScene::OnActivate(ICameraControl* cameraControl)
{
    cameraControl->SetCameraConfig(6.0f, 0.2f, 0.5f);
}

void OtherScene::OnGuiRender()
{
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Other Shaders:");
    ImGui::BulletText("Dissolve Shader");
    ImGui::BulletText("Ripple Shader");
    ImGui::BulletText("Spherify Shader");
    ImGui::BulletText("Explosion Shader");
}