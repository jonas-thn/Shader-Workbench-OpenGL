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
    void Draw() override;

    std::string GetName() const override { return "Node Editor"; }
    void OnActivate(IApplicationContext* cameraControl) override;
    void OnGuiRender() override;

private:
    void AddNode(NodeType type);
    void DrawNode(Node& node);
    void DrawPins(std::vector<int>& inputPins, std::vector<int>& outputPins);
    void DrawNodeEditor();
    void DrawNodePopup(bool& popupOpen);
    void HandleLinks();
    void HandleNodeDeletion();
    void DeleteNode(int nodeId);

	int FindLinkConnectedToInput(int inputPin);
    int FindNodeByOutput(int outputPin);
	glm::vec3 EvaluateNode(int nodeIndex); 
    void CompileNodes();

private:
    Shader nodeShader;
    Mesh cube;

    IApplicationContext* appContext = nullptr;

    std::vector<Node> nodes;
    std::vector<Link> links;
    int currentId = 1;
	glm::vec3 nodeColor = glm::vec3(-1.0f);
};

