#pragma once

#include <vector>
#include <string> 
#include <glm/glm.hpp> 
#include "../Shader.h"
#include "../Mesh.h"
#include "../ICameraControl.h"

class Application;

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void Update(float dt) = 0;

    virtual void Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution) = 0;

    virtual std::string GetName() const = 0;

    virtual void OnGuiRender() {}

    virtual void OnActivate(ICameraControl* cameraControl) {}

protected:
    std::vector<Mesh*> meshList;
};