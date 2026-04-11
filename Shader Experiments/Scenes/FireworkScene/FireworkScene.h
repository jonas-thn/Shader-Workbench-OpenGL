#pragma once

#include "../Scene.h"
#include "../../MeshData.h"
#include <vector>

class Application;

class FireworkScene : public Scene
{
public:
    FireworkScene();
    virtual ~FireworkScene() = default;

    void Update(float dt) override;
    void Draw() override;

    std::string GetName() const override { return "Firework"; }
    void OnActivate(IApplicationContext* cameraControl) override;
    void OnGuiRender() override;

private:
    Shader fireworkShader;
    Mesh screen;
};