#pragma once

#include "../Scene.h"
#include "../../MeshData.h"
#include <vector>

class Application;

class PlanetScene : public Scene
{
public:
    PlanetScene();
    virtual ~PlanetScene() = default;

    void Update(float dt) override;
    void Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution) override;

    std::string GetName() const override { return "Planet"; }
    void OnActivate(ICameraControl* cameraControl) override;
    void OnGuiRender() override;

private:
    Shader planetShader;
    Mesh screen;
};