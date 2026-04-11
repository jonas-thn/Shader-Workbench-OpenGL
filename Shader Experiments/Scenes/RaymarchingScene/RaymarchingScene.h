#pragma once

#include "../Scene.h"
#include "../../MeshData.h"
#include <vector>

class Application;

class RaymarchingScene : public Scene
{
public:
    RaymarchingScene();
    virtual ~RaymarchingScene() = default;

    void Update(float dt) override;
    void Draw() override;

    std::string GetName() const override { return "Raymarching"; }
    void OnActivate(IApplicationContext* cameraControl) override;
    void OnGuiRender() override;

private:
    Shader raymarchShader;

    Mesh screen;
};