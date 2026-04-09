#include "NodeScene.h"
#include "../../Application.h"
#include "../../ImGui/imnodes.h"

NodeScene::NodeScene() : 
    nodeShader("./Shader/Standard/standardShader.vert", "./Shader/Standard/standardShader.frag"),
    cube("./Models/cube.obj")
{
    meshList.push_back(&cube);
	AddNode(Master);
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
    ImNodes::BeginNodeEditor();

    for (auto& node : nodes)
    {
        DrawNode(node);
    }

    ImNodes::EndNodeEditor();
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

    if(popupOpen)
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

    ImGui::SameLine();

    if (ImGui::Button("Render"))
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
        newNode.outputPins.push_back(currentId++);
        newNode.outputPins.push_back(currentId++);
        newNode.outputPins.push_back(currentId++);
        break;
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


