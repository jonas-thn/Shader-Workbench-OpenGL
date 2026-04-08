#pragma once

#include "../Scene.h"
#include "../../MeshData.h"
#include "../../Mesh.h"
#include <vector>
#include <string>

class CustomScene : public Scene
{
public:
    CustomScene();
    virtual ~CustomScene() = default;

    void Update(float dt) override;
    void Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution) override;

    std::string GetName() const override { return "Custom"; }
    void OnActivate(ICameraControl* cameraControl) override;
    void OnGuiRender() override;

private:
    Shader customShader;
    Mesh screen;

	const size_t extraSize = 8192;
    std::vector<char> textBuffer;
	std::string defaultShaderCodeFilepath = "./Shader/Custom/customDefault.frag";
    void LoadDefaultShaderCode();
	void SaveShaderCodeToFile(const std::string& filepath);
};

