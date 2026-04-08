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
    void Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution) override;

    std::string GetName() const override { return "Terrain"; }
    void OnActivate(IApplicationContext* cameraControl) override;
    void OnGuiRender() override;

private:
    Shader terrainShader;

    Mesh screen;
};