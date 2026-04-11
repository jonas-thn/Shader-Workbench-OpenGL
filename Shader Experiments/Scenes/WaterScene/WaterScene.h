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
    void Draw() override;

    std::string GetName() const override { return "Water"; }
    void OnActivate(IApplicationContext* cameraControl) override;
    void OnGuiRender() override;

private:
    Shader waterShader;
    Mesh screen;
};