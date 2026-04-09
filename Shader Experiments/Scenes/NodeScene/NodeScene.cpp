#include "NodeScene.h"
#include "../../Application.h"
#include "../../ImGui/imnodes.h"

NodeScene::NodeScene()
{

}

void NodeScene::Update(float dt)
{

}

void NodeScene::Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos, float time, const glm::vec2& resolution)
{

}

void NodeScene::OnActivate(IApplicationContext* appContext)
{
	this->appContext = appContext;
	this->appContext->SetCameraConfig(2.5f, 1.0f, 0.0f);
}

void NodeScene::OnGuiRender()
{
    float availableHeight = ImGui::GetContentRegionAvail().y;
    float nodeEditorHeight = availableHeight - ImGui::GetFrameHeightWithSpacing();
	ImGui::SetNextWindowSize(ImVec2(-1.0f, nodeEditorHeight));

    ImGui::BeginChild("NodeEditorRegion", ImVec2(-1, nodeEditorHeight));

    ImNodes::BeginNodeEditor();
    ImNodes::BeginNode(1);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted("simple node :)");
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(2);
    ImGui::Text("input");
    ImNodes::EndInputAttribute();

    ImNodes::BeginOutputAttribute(3);
    ImGui::Indent(40);
    ImGui::Text("output");
    ImNodes::EndOutputAttribute();

    ImNodes::EndNode();
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

    if (ImGui::Button("Render"))
    {

    }
}
