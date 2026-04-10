#include "NodeScene.h"
#include "../../Application.h"
#include "../../ImGui/imnodes.h"
#include <algorithm>

// 5. Nodes Rendern / Shader

NodeScene::NodeScene() : 
    nodeShader("./Shader/Standard/standardShader.vert", "./Shader/Standard/standardShader.frag"),
    cube("./Models/cube.obj")
{
    meshList.push_back(&cube);
	AddNode(Master);

	static bool enableLinkDetach = true;
	ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &enableLinkDetach;
}

void NodeScene::Update(float dt) {}

void NodeScene::Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution)
{
    for (int i = 0; i < meshList.size(); i++)
    {
        meshList[i]->Draw(nodeShader, view, projection, camPos, time, resolution, i, 0);
    }
}

void NodeScene::OnActivate(IApplicationContext* appContext)
{
	this->appContext = appContext;
	this->appContext->SetCameraConfig(3.5f, 0.5f, 1.0f);
}

void NodeScene::OnGuiRender()
{
    float availableHeight = ImGui::GetContentRegionAvail().y;
    float nodeEditorHeight = availableHeight - ImGui::GetFrameHeightWithSpacing();
	ImGui::SetNextWindowSize(ImVec2(-1.0f, nodeEditorHeight));

    ImGui::BeginChild("NodeEditorRegion", ImVec2(-1, nodeEditorHeight));
    
    DrawNodeEditor();

    HandleLinks();
    HandleNodeDeletion();

    ImGui::EndChild();

    if (ImGui::Button("Swap"))
    {
        if (appContext)
        {
            appContext->ToggleWindowLayout();
        }
    }

    ImGui::SameLine();

    static bool popupOpen = false;
    if (ImGui::Button("Add Node"))
    {
        popupOpen = true;
    }
	DrawNodePopup(popupOpen);

    ImGui::SameLine();

    if (ImGui::Button("Reset"))
    {
        for (auto node : nodes)
        {
            DeleteNode(node.id);
        }
         nodes.clear();
         links.clear();
         currentId = 1;
		 AddNode(Master);
    }

    ImGui::SameLine();

    if (ImGui::Button("Compile"))
    {

    }
}

void NodeScene::AddNode(NodeType type)
{
    Node newNode;
    newNode.id = currentId++;
    newNode.type = type;

    switch (type)
    {
    case Master:
		newNode.inputPins.push_back(currentId++);
        break;
    case Time:
        newNode.outputPins.push_back(currentId++);
        break;
    case Color:
    case Value:
        newNode.outputPins.push_back(currentId++);
        break;
    case Add:
    case Subtract:
    case Mutliply:
        newNode.inputPins.push_back(currentId++);
        newNode.inputPins.push_back(currentId++);
        newNode.outputPins.push_back(currentId++);
		break;
    case Sin:
        newNode.inputPins.push_back(currentId++);
        newNode.outputPins.push_back(currentId++);
        break;
    default:
        break;
    }

	nodes.push_back(newNode);
}

void NodeScene::DrawNode(Node& node)
{
    ImNodes::BeginNode(node.id);
    ImNodes::BeginNodeTitleBar();

    switch (node.type)
    {
    case Master:
        ImGui::TextUnformatted("Master");
        break;
    case Time:
        ImGui::TextUnformatted("Time");
        break;
    case Color:
		ImGui::TextUnformatted("Color");
        break;
    case Add:
        ImGui::TextUnformatted("Add");
        break;
    case Subtract:
        ImGui::TextUnformatted("Subtract");
        break;
    case Mutliply:
		ImGui::TextUnformatted("Mutliply");
        break;
    case Value:
        ImGui::TextUnformatted("Value");
        break;
    case Sin:
        ImGui::TextUnformatted("Sin");
        break;
    default:
        ImGui::TextUnformatted("NODE ERROR");
        break;
    }

    ImNodes::EndNodeTitleBar();

    DrawPins(node.inputPins, node.outputPins);

    switch (node.type)
    {
    case NodeType::Color:
        ImNodes::BeginOutputAttribute(node.outputPins[0]);

        ImGui::SetNextItemWidth(60.0f);
        ImGui::ColorPicker3("##colorPicker", &node.color[0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);

        ImNodes::EndOutputAttribute();
        break;

    case NodeType::Value:
        ImNodes::BeginOutputAttribute(node.outputPins[0]);

        ImGui::SetNextItemWidth(60.0f);
        ImGui::DragFloat("##floatVal", &node.value, 0.01f);

        ImNodes::EndOutputAttribute();
        break;
    }


    ImNodes::EndNode();
}

void NodeScene::DrawPins(std::vector<int>& inputPins, std::vector<int>& outputPins)
{
    for (auto inputPin : inputPins)
    {
        ImNodes::BeginInputAttribute(inputPin);
        ImGui::Text("in");
        ImNodes::EndInputAttribute();
    }
    for (auto outputPin : outputPins)
    {
        ImNodes::BeginOutputAttribute(outputPin);
        ImGui::Indent(40);
        ImGui::Text("out");
        ImNodes::EndOutputAttribute();
    }
}

void NodeScene::DrawNodeEditor()
{
    ImNodes::BeginNodeEditor();

    for (auto& node : nodes)
    {
        DrawNode(node);
    }

    for (const auto& link : links)
    {
        ImNodes::Link(link.id, link.startPin, link.endPin);
    }

    ImNodes::EndNodeEditor();
}

void NodeScene::DrawNodePopup(bool& popupOpen)
{
    if (popupOpen)
    {
        ImGui::OpenPopup("AddNodeMenu");
    }

    if (ImGui::BeginPopup("AddNodeMenu"))
    {
        ImGui::SeparatorText("Inputs");
        if (ImGui::MenuItem("Value Node"))
        {
            AddNode(Value);
            popupOpen = false;
        }
        if (ImGui::MenuItem("Time Node"))
        {
            AddNode(Time);
            popupOpen = false;
        }
        if (ImGui::MenuItem("Color Node"))
        {
            AddNode(Color);
            popupOpen = false;
        }

        ImGui::SeparatorText("Math");
        if (ImGui::MenuItem("Add Node"))
        {
            AddNode(Add);
            popupOpen = false;
        }
        if (ImGui::MenuItem("Subtract Node"))
        {
            AddNode(Subtract);
            popupOpen = false;
        }
        if (ImGui::MenuItem("Multiply Node"))
        {
            AddNode(Mutliply);
            popupOpen = false;
        }
        if (ImGui::MenuItem("Sin Node"))
        {
            AddNode(Sin);
            popupOpen = false;
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::MenuItem("Exit"))
        {
            popupOpen = false;
        }

        ImGui::EndPopup();
    }
}

void NodeScene::HandleLinks()
{
    int startPinId, endPinId;
    if (ImNodes::IsLinkCreated(&startPinId, &endPinId))
    {
        Link newLink;
        newLink.id = currentId++;
        newLink.startPin = startPinId;
        newLink.endPin = endPinId;
        links.push_back(newLink);
    }

    int destroyedLinkId;
    if (ImNodes::IsLinkDestroyed(&destroyedLinkId))
    {
        links.erase(std::remove_if(links.begin(), links.end(), [destroyedLinkId](const Link& link) {
            return link.id == destroyedLinkId;
            }), links.end());
    }
}

void NodeScene::HandleNodeDeletion()
{
    if (ImGui::IsKeyPressed(ImGuiKey_Delete) || ImGui::IsKeyPressed(ImGuiKey_Backspace))
    {
		const int numSelectedNodes = ImNodes::NumSelectedNodes();

        if(numSelectedNodes > 0)
        {
            std::vector<int> selectedNodes(numSelectedNodes);
			ImNodes::GetSelectedNodes(selectedNodes.data());

            for (int nodeId : selectedNodes)
            {
                DeleteNode(nodeId);
			}

            ImNodes::ClearNodeSelection();
		}
    }
}

void NodeScene::DeleteNode(int nodeId)
{
    int nodeIndex = -1;
    for (int i = 0; i < nodes.size(); i++)
    {
        if(nodes[i].id == nodeId)
        {
            nodeIndex = i;
            break;
	    }
    }

    if (nodeIndex == -1 || nodes[nodeIndex].type == Master)
    {
        return;
    }

	Node& nodeToDelete = nodes[nodeIndex];

    for (int i = links.size() - 1; i >= 0; i--)
    {
        Link& currentLink = links[i];
        bool linkMustBeDeleted = false;

        for (int inputPin : nodeToDelete.inputPins)
        {
            if (currentLink.endPin == inputPin)
            {
                linkMustBeDeleted = true;
                break;
            }
		}

        for (int outputPin : nodeToDelete.outputPins)
        {
            if (currentLink.startPin == outputPin)
            {
                linkMustBeDeleted = true;
				break;
            }
        }

        if (linkMustBeDeleted)
        {
            links.erase(links.begin() + i);
        }
    }

    nodes.erase(nodes.begin() + nodeIndex);
}


