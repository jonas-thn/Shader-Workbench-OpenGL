#pragma once

#include "../Scene.h"
#include "../../MeshData.h"
#include "../../Mesh.h"
#include <vector>
#include <string>

class Application;

class NodeScene : public Scene
{
public:
    NodeScene();
    ~NodeScene() = default;

    void Update(float dt) override;
    void Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution) override;

    std::string GetName() const override { return "Node Editor"; }
    void OnActivate(IApplicationContext* cameraControl) override;
    void OnGuiRender() override;

private:
    /*Shader planetShader;
    Mesh screen;*/
    IApplicationContext* appContext = nullptr;
};

