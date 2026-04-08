#pragma once

#include "../Scene.h"

class Application;

class LightingScene : public Scene
{
public:
    LightingScene();
    virtual ~LightingScene() = default;

    void Update(float dt) override;
    void Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution) override;

    std::string GetName() const override { return "Lighting"; }
    void OnActivate(IApplicationContext* cameraControl) override;
    void OnGuiRender() override;

private:
    Shader lightingShader;

    Mesh pyramid;
    Mesh sphere;
    Mesh cone;
    Mesh cube;
    Mesh ring;

    float speed = 2.0f;

    float upperBound = 5.01f;
    float lowerBound = -5.01f;

    float pyramidPos = 5.0f;
    float spherePos = 3.0f;
    float conePos = 1.0f;
    float cubePos = -1.0f;
    float ringPos = -3.0f;

    void MoveMesh(float& pos, Mesh& mesh, float dt);
};