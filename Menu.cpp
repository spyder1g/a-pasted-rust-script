#include "Menu.hpp"
#include "imgui-docking/imgui.h"
#include "imgui-docking/imgui_internal.h"
#include "Script.hpp"

int Menu::menuWindowHeight = 280;
int Menu::menuWindowWidth = 460;
const char* Menu::menuWindowName = "...";

void Menu::SetStyles() {
    ImGuiStyle& style = ImGui::GetStyle();

    static ImVec4 menucolor = ImVec4(0.9f, 0.13f, 0.9f, 1.00f);
    static ImVec4 COLORSHOW = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);

    static ImVec4 color = ImColor(17, 207, 200);
    static ImVec4 backgroundcolor = ImVec4(27, 28, 36, 1.00f);
    static ImVec4 textcolor2 = ImVec4(1.f, 1.f, 1.f, 1.00f);
    static ImVec4 textcolor1 = ImVec4(1.f, 1.f, 1.f, 1.00f);
    static bool alpha_preview = true;
    static bool alpha_half_preview = false;
    static bool drag_and_drop = true;
    static bool options_menu = true;
    static bool hdr = false;
    ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImColor(255, 255, 255);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.43f, 0.43f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImColor(27, 28, 36); // background
    colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.00f); // ImVec4(0.12f, 0.12f, 0.15f, 1.00f)
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_Border] = ImColor(13, 14, 18);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImColor(27, 28, 36);
    colors[ImGuiCol_FrameBgHovered] = ImColor(27, 28, 36);;
    colors[ImGuiCol_FrameBgActive] = color;
    colors[ImGuiCol_TitleBg] = ImColor(0, 0, 0);
    colors[ImGuiCol_TitleBgActive] = ImColor(0, 0, 0);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.48f, 0.07f, 0.20f, 0.35f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImColor(222, 38, 38);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.48f, 0.07f, 0.20f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = color;
    colors[ImGuiCol_ButtonActive] = color;
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.48f, 0.07f, 0.20f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.07f, 0.20f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.48f, 0.07f, 0.20f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.48f, 0.07f, 0.20f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.48f, 0.07f, 0.20f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.48f, 0.07f, 0.20f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.48f, 0.07f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.48f, 0.07f, 0.20f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);

    style.Alpha = 1.0f;               // Global alpha applies to everything in ImGui
    style.WindowPadding = ImVec2(10, 10);     // Padding within a window
    style.WindowMinSize = ImVec2(100, 100);   // Minimum window size
    style.WindowRounding = 0.0f;               // Radius of window corners rounding. Set to 0.0f to have rectangular windows
    style.WindowTitleAlign = ImVec2(0.0f, 0.0f); // Alignment for title bar text
    style.FramePadding = ImVec2(13, 0);       // Padding within a framed rectangle (used by most widgets)
    style.FrameRounding = 0.0f;               // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
    style.ItemSpacing = ImVec2(5, 5);       // Horizontal and vertical spacing between widgets/lines
    style.ItemInnerSpacing = ImVec2(4, 4);       // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
    style.TouchExtraPadding = ImVec2(0, 0);       // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
    style.IndentSpacing = 18.0f;              // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
    style.ColumnsMinSpacing = 6.0f;               // Minimum horizontal spacing between two columns
    style.ScrollbarSize = 16.0f;              // Width of the vertical scrollbar, Height of the horizontal scrollbar
    style.ScrollbarRounding = 9.0f;               // Radius of grab corners rounding for scrollbar
    style.GrabMinSize = 10.0f;              // Minimum width/height of a grab box for slider/scrollbar
    style.GrabRounding = 0.0f;               // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f); // Alignment of button text when button is larger than text.
    style.DisplayWindowPadding = ImVec2(22, 22);     // Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
    style.DisplaySafeAreaPadding = ImVec2(4, 4);       // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
    style.AntiAliasedLines = true;               // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
    style.CurveTessellationTol = 1.25f;              // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.WindowBorderSize = 1.0f;
    //style.TabBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.ScrollbarSize = 0.1f;
    style.ScrollbarRounding = 0;
}