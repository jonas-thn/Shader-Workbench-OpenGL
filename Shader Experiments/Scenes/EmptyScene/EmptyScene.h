#pragma once

#include "../Scene.h"

class Application;

class EmptyScene : public Scene
{
public:
    EmptyScene();
    virtual ~EmptyScene() = default;

    void Update(float dt) override;
    void Draw() override;

    std::string GetName() const override { return "Empty"; }

    void OnActivate(IApplicationContext* cameraControl) override;

    void OnGuiRender() override;

private:
    Shader standardShader;
    Mesh text;
};