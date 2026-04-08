#include "GrassScene.h"
#include "../../Application.h"

GrassScene::GrassScene() : 
    standardShader("./Shader/Standard/standardShader.vert", "./Shader/Standard/standardShader.frag"),
    grassShader("./Shader/Grass/grassShader.vert", "./Shader/Grass/grassShader.frag"),

    plane("./Models/quad.obj"),
    sphere("./Models/sphere.obj"),

    grass1("./Models/grass.obj"),
    grass2("./Models/grass.obj")
{
    plane.Translate(glm::vec3(0.0, -0.5, 0.0));
    plane.Rotate(90.0f, glm::vec3(1.0, 0.0, 0.0));
    plane.SetScale(glm::vec3(4.0));

    sphere.Translate(glm::vec3(1.0, 2.0, -2.0));
    sphere.SetScale(glm::vec3(0.5));

     meshList.push_back(&plane);
     meshList.push_back(&sphere);
}

void GrassScene::Update(float dt) {}

void GrassScene::Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution)
{
    plane.Draw(standardShader, view, projection, camPos, time, resolution, 800, 0);

    grass1.Draw(grassShader, view, projection, camPos, time, resolution, 0, count);
    grass2.Draw(grassShader, view, projection, camPos, time, resolution, 1, count);
}

void GrassScene::OnActivate(ICameraControl* cameraControl)
{
    cameraControl->SetCameraConfig(6.0f, 0.3f, 1.5f);
}

void GrassScene::OnGuiRender()
{
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Core Technique:");
    ImGui::BulletText("GPU Instancing");             
    ImGui::BulletText("Procedural Vertex Displacement"); 
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Animation & Physics:");
    ImGui::BulletText("Sine-Wave Wind Simulation");  
    ImGui::BulletText("Dynamic Normal Recalculation");
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Visuals:");
    ImGui::BulletText("3D Gradient Noise");          
    ImGui::BulletText("Height-based Color Gradient");
}