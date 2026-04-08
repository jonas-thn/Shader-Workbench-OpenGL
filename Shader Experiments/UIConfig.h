#pragma once
#include "ImGui/imgui.h"

inline void InitImGuiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsDark();

    ImVec4 blackColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    ImVec4 grey02Color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    ImVec4 grey04Color = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    ImVec4 grey06Color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
    ImVec4 grey08Color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    ImVec4 whiteColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    style.Colors[ImGuiCol_FrameBg] = grey02Color;
    style.Colors[ImGuiCol_FrameBgHovered] = grey04Color;
    style.Colors[ImGuiCol_FrameBgActive] = grey06Color;
    style.Colors[ImGuiCol_PopupBg] = grey02Color;
    style.Colors[ImGuiCol_Border] = grey04Color;
    style.Colors[ImGuiCol_BorderShadow] = blackColor;
    style.Colors[ImGuiCol_Header] = grey04Color;
    style.Colors[ImGuiCol_HeaderHovered] = grey06Color;
    style.Colors[ImGuiCol_HeaderActive] = grey08Color;
    style.Colors[ImGuiCol_Text] = whiteColor;
    style.Colors[ImGuiCol_CheckMark] = whiteColor;
    style.Colors[ImGuiCol_ScrollbarBg] = grey02Color;
    style.Colors[ImGuiCol_ScrollbarGrab] = grey04Color;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = grey06Color;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = grey08Color;
    style.Colors[ImGuiCol_Button] = grey04Color;
    style.Colors[ImGuiCol_SliderGrab] = grey08Color;
    style.Colors[ImGuiCol_SliderGrabActive] = whiteColor;
    style.Colors[ImGuiCol_ButtonHovered] = grey06Color;
    style.Colors[ImGuiCol_ButtonActive] = grey08Color;
    style.Colors[ImGuiCol_TextSelectedBg] = grey06Color;
    style.Colors[ImGuiCol_Separator] = grey04Color;
    style.Colors[ImGuiCol_SeparatorHovered] = grey06Color;
    style.Colors[ImGuiCol_SeparatorActive] = grey08Color;
    style.Colors[ImGuiCol_TitleBg] = grey02Color;
    style.Colors[ImGuiCol_TitleBgActive] = grey04Color;
    style.Colors[ImGuiCol_TitleBgCollapsed] = grey02Color;
    style.Colors[ImGuiCol_ResizeGrip] = grey04Color;
    style.Colors[ImGuiCol_ResizeGripHovered] = grey06Color;
    style.Colors[ImGuiCol_ResizeGripActive] = grey08Color;
    style.Colors[ImGuiCol_DragDropTarget] = grey08Color;
    style.Colors[ImGuiCol_NavHighlight] = grey06Color;
    style.Colors[ImGuiCol_NavWindowingHighlight] = grey06Color;
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0, 0, 0, 0.4f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.4f);
    style.WindowPadding = ImVec2(20.0f, 20.0f);
}