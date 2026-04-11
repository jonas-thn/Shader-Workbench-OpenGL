#include "CustomScene.h"
#include "../../Application.h" 
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

CustomScene::CustomScene() :
    customShader("./Shader/Custom/customShader.vert", "./Shader/Custom/customDefault.frag"),
    screen(std::vector<float>(std::begin(screenVertices), std::end(screenVertices)), {})
{
    meshList.push_back(&screen);

    LoadDefaultShaderCode();
}

void CustomScene::Update(float dt) {}

void CustomScene::Draw()
{
    for (int i = 0; i < meshList.size(); i++)
    {
        meshList[i]->Draw(customShader, i, 0);
    }
}

void CustomScene::OnActivate(IApplicationContext* appContext)
{
	this->appContext = appContext;
    this->appContext->SetCameraConfig(2.5f, 1.0f, 0.0f);
}

void CustomScene::OnGuiRender()
{
    float availableHeight = ImGui::GetContentRegionAvail().y;
    float textBoxHeight = availableHeight - ImGui::GetFrameHeightWithSpacing();
	ImGui::InputTextMultiline("###Shader Code", textBuffer.data(), textBuffer.size(), ImVec2(-1.0f, textBoxHeight), ImGuiInputTextFlags_AllowTabInput);

    if (ImGui::Button("Swap"))
    {
        if (appContext)
        {
            appContext->ToggleWindowLayout();
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset"))
    {
		LoadDefaultShaderCode();
    }

    ImGui::SameLine();

    if (ImGui::Button("Complile"))
    {
		SaveShaderCodeToFile("./Shader/Custom/customShader.frag");
		customShader = Shader("./Shader/Custom/customShader.vert", "./Shader/Custom/customShader.frag");
    }
}

void CustomScene::LoadDefaultShaderCode()
{
    std::ifstream file(defaultShaderCodeFilepath);
    std::string content;

    if (file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
    }

    textBuffer.resize(content.size() + extraSize);

    std::copy(content.begin(), content.end(), textBuffer.begin());

    textBuffer[content.size()] = '\0';
}

void CustomScene::SaveShaderCodeToFile(const std::string& filepath)
{
    std::ofstream file(filepath);

    if (!file.is_open())
    {
        std::cerr << "Fehler: Konnte Datei nicht öffnen: " << filepath << std::endl;
        return;
    }

    file << textBuffer.data();
}
