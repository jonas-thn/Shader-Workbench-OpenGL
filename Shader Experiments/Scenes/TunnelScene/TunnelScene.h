#pragma once

#include "../Scene.h"
#include "../../MeshData.h"
#include <vector>

class Application;

class TunnelScene : public Scene
{
public:
    TunnelScene();
    virtual ~TunnelScene() = default;

    void Update(float dt) override;
    void Draw() override;

    std::string GetName() const override { return "Tunnel"; }
    void OnActivate(IApplicationContext* cameraControl) override;
    void OnGuiRender() override;

private:
    Shader tunnelShder;
    Mesh screen;
};