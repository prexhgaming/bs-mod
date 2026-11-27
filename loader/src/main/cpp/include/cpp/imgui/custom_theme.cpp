#pragma once

#include "../../hpp/imgui/custom_theme.h"

void StyleColorsCustom(ImGuiStyle* _style) {
    ImGuiStyle& style = _style ? *_style : ImGui::GetStyle();
    
    // ===== MODERN STYLING =====
    style.WindowRounding = 12.0f;
    style.FrameRounding = 8.0f;
    style.PopupRounding = 8.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding = 6.0f;
    style.TabRounding = 8.0f;
    style.ChildRounding = 8.0f;
    
    // Border sizes
    style.WindowBorderSize = 1.5f;
    style.FrameBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    
    // Padding
    style.WindowPadding = ImVec2(15, 15);
    style.FramePadding = ImVec2(12, 8);
    style.ItemSpacing = ImVec2(10, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    
    // ===== PREMIUM COLOR SCHEME =====
    
    // Backgrounds
    style.Colors[ImGuiCol_WindowBg] = ImColor(18, 18, 24, 240);
    style.Colors[ImGuiCol_ChildBg] = ImColor(25, 25, 32, 200);
    style.Colors[ImGuiCol_PopupBg] = ImColor(20, 20, 28, 245);
    
    // Borders
    style.Colors[ImGuiCol_Border] = ImColor(45, 85, 135, 150);
    style.Colors[ImGuiCol_BorderShadow] = ImColor(0, 0, 0, 0);
    
    // Text
    style.Colors[ImGuiCol_Text] = ImColor(240, 240, 245, 255);
    style.Colors[ImGuiCol_TextDisabled] = ImColor(100, 100, 110, 255);
    
    // Headers
    style.Colors[ImGuiCol_Header] = ImColor(35, 45, 70, 200);
    style.Colors[ImGuiCol_HeaderHovered] = ImColor(45, 60, 90, 220);
    style.Colors[ImGuiCol_HeaderActive] = ImColor(50, 70, 110, 240);
    
    // Buttons
    style.Colors[ImGuiCol_Button] = ImColor(40, 50, 80, 200);
    style.Colors[ImGuiCol_ButtonHovered] = ImColor(50, 65, 100, 220);
    style.Colors[ImGuiCol_ButtonActive] = ImColor(60, 80, 120, 240);
    
    // Frames
    style.Colors[ImGuiCol_FrameBg] = ImColor(30, 35, 50, 180);
    style.Colors[ImGuiCol_FrameBgHovered] = ImColor(35, 45, 65, 200);
    style.Colors[ImGuiCol_FrameBgActive] = ImColor(40, 55, 80, 220);
    
    // Tabs - Modern look
    style.Colors[ImGuiCol_Tab] = ImColor(30, 35, 50, 200);
    style.Colors[ImGuiCol_TabHovered] = ImColor(40, 50, 75, 220);
    style.Colors[ImGuiCol_TabActive] = ImColor(50, 70, 110, 240);
    style.Colors[ImGuiCol_TabUnfocused] = ImColor(20, 25, 40, 180);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(30, 40, 65, 220);
    
    // Title bars
    style.Colors[ImGuiCol_TitleBg] = ImColor(25, 30, 45, 255);
    style.Colors[ImGuiCol_TitleBgActive] = ImColor(35, 45, 70, 255);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(20, 25, 40, 220);
    
    // Menu bar
    style.Colors[ImGuiCol_MenuBarBg] = ImColor(30, 35, 50, 240);
    
    // Scrollbars
    style.Colors[ImGuiCol_ScrollbarBg] = ImColor(20, 25, 35, 150);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(60, 80, 120, 180);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(70, 90, 140, 200);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(80, 100, 160, 220);
    
    // Checkmarks & Sliders
    style.Colors[ImGuiCol_CheckMark] = ImColor(0, 220, 150, 255);
    style.Colors[ImGuiCol_SliderGrab] = ImColor(0, 180, 255, 200);
    style.Colors[ImGuiCol_SliderGrabActive] = ImColor(0, 220, 255, 255);
    
    // Selection
    style.Colors[ImGuiCol_TextSelectedBg] = ImColor(0, 120, 215, 100);
    
    // Drag and drop
    style.Colors[ImGuiCol_DragDropTarget] = ImColor(255, 255, 0, 150);
    
    // Navigation
    style.Colors[ImGuiCol_NavHighlight] = ImColor(0, 150, 255, 150);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImColor(255, 255, 255, 180);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImColor(200, 200, 200, 50);
    
    // Modal
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImColor(10, 10, 15, 150);
    
    // Separators
    style.Colors[ImGuiCol_Separator] = ImColor(60, 80, 120, 150);
    style.Colors[ImGuiCol_SeparatorHovered] = ImColor(80, 100, 160, 200);
    style.Colors[ImGuiCol_SeparatorActive] = ImColor(100, 120, 180, 255);
    
    // Plot lines
    style.Colors[ImGuiCol_PlotLines] = ImColor(150, 200, 255, 255);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImColor(255, 150, 0, 255);
    style.Colors[ImGuiCol_PlotHistogram] = ImColor(0, 200, 255, 255);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImColor(255, 100, 0, 255);
    
    // ===== ADVANCED STYLING =====
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 12.0f;
    style.WindowMenuButtonPosition = ImGuiDir_Right;
    
    // Tab styling
    style.TabBorderSize = 1.0f;

    
    // Disabled opacity
    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.6f;
}
