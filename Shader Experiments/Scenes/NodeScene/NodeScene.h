#pragma once

#include "../Scene.h"
#include "../../MeshData.h"
#include "../../Mesh.h"
#include <vector>
#include <string>
#include "../NodeModel.h"

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
    void AddNode(NodeType type);
    void DrawNode(Node& node);
    void DrawPins(std::vector<int>& inputPins, std::vector<int>& outputPins);

private:
    Shader nodeShader;
    Mesh cube;

    IApplicationContext* appContext = nullptr;

    std::vector<Node> nodes;
    std::vector<Link> links;
    int currentId = 1;
};

