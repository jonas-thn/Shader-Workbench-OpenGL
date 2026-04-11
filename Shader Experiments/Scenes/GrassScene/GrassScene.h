#pragma once

#include "../Scene.h"
#include "../../Shader.h" 
#include "../../Mesh.h"
#include <vector>

class Application;

class GrassScene : public Scene
{
public:
    GrassScene();
    virtual ~GrassScene() = default;

    void Update(float dt) override;

    void Draw() override;

    std::string GetName() const override { return "Grass"; }
    void OnActivate(IApplicationContext* cameraControl) override;
    void OnGuiRender() override;

private:
    Shader standardShader;
    Shader grassShader;

    Mesh plane;
    Mesh sphere;

    Mesh grass1;
    Mesh grass2;

    int count = 900;
};