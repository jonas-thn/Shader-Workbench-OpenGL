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
    void Draw() override;

    std::string GetName() const override { return "Planet"; }
    void OnActivate(IApplicationContext* cameraControl) override;
    void OnGuiRender() override;

private:
    Shader planetShader;
    Mesh screen;
};