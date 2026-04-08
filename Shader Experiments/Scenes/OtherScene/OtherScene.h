#pragma once

#include "../Scene.h"
#include "../../MeshData.h"
#include "../../Mesh.h"
#include <vector>

class Application;

class OtherScene : public Scene
{
public:
    OtherScene();
    virtual ~OtherScene() = default;

    void Update(float dt) override;
    void Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution) override;

    std::string GetName() const override { return "Experimental"; }
    void OnActivate(ICameraControl* cameraControl) override;
    void OnGuiRender() override;

private:
    Shader noiseShader;
    Shader rippleShader;

    Mesh sphereNoise;
    Mesh sphereRipple;
    Mesh monkeyExplode;
    Mesh monkeySphere;
};