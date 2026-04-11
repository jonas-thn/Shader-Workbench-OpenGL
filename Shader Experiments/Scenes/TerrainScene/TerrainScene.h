#pragma once

#include "../Scene.h"
#include "../../MeshData.h"
#include <vector>

class Application;

class TerrainScene : public Scene
{
public:
    TerrainScene();
    virtual ~TerrainScene() = default;

    void Update(float dt) override;
    void Draw() override;

    std::string GetName() const override { return "Terrain"; }
    void OnActivate(IApplicationContext* cameraControl) override;
    void OnGuiRender() override;

private:
    Shader terrainShader;

    Mesh screen;
};