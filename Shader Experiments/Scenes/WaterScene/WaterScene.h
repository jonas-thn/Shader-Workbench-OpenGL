#pragma once

#include "../Scene.h"
#include "../../MeshData.h"
#include <vector>

class Application;

class WaterScene : public Scene
{
public:
    WaterScene();
    virtual ~WaterScene() = default;

    void Update(float dt) override;
    void Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution) override;

    std::string GetName() const override { return "Water"; }
    void OnActivate(ICameraControl* cameraControl) override;
    void OnGuiRender() override;

private:
    Shader waterShader;
    Mesh screen;
};