#pragma once

#include "../Scene.h"

class Application;

class EmptyScene : public Scene
{
public:
    EmptyScene();
    virtual ~EmptyScene() = default;

    void Update(float dt) override;
    void Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution) override;

    std::string GetName() const override { return "Empty"; }

    void OnActivate(IApplicationContext* cameraControl) override;

    void OnGuiRender() override;

private:
    Shader standardShader;
    Mesh text;
};