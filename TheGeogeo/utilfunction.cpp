#include "includes.h"
#include <psapi.h>
#include <cstdint>

namespace utilFunction
{
	uintptr_t pattern_scan(std::uintptr_t Address, const char* Signature, bool Relative)
	{
		auto pattern_to_byte = [](const char* pattern) {
			auto bytes = std::vector<int>{};
			const auto start = const_cast<char*>(pattern);
			const auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					++current;
					bytes.push_back(-1);
				}
				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
			};

		const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(Address);
		const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(Address) + dos_header->e_lfanew);

		const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
		const auto pattern_bytes = pattern_to_byte(Signature);
		const auto scan_bytes = reinterpret_cast<std::uint8_t*>(Address);

		const auto s = pattern_bytes.size();
		const auto d = pattern_bytes.data();

		for (auto i = 0ul; i < size_of_image - s; ++i) {
			bool found = true;
			for (auto j = 0ul; j < s; ++j) {
				if (scan_bytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}
			}

			if (found) {
				const auto offset = *reinterpret_cast<int*>(&scan_bytes[i] + 3);

				return Relative ? reinterpret_cast<uintptr_t>(&scan_bytes[i]) + offset + 7 : reinterpret_cast<uintptr_t>(&scan_bytes[i]);
			}
		}

		return 0;
	}

	static inline ImVec4 HSV(float h, float s, float v, float a = 1.0f) {
		float r, g, b;
		ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
		return ImVec4(r, g, b, a);
	}
	static inline ImVec4 Lerp(const ImVec4& a, const ImVec4& b, float t) {
		return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
	}

	void ApplyNeoCarbonStyle(ImGuiStyle* style,
		float uiScale = 1.0f,
		float accentHueHSV = 0.55f,     // main accent (0.83f = purple)
		float menuAccentIntensity = 0.30f)  // how much accent is applied to title/menu
	{
		IM_ASSERT(style);

		// Palette (layered backgrounds, high-contrast text)
		const ImVec4 colBgBase = ImVec4(0.07f, 0.08f, 0.10f, 1.00f);  // Deep base
		const ImVec4 colBgWindow = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);  // Window
		const ImVec4 colBgFrame = ImVec4(0.14f, 0.15f, 0.18f, 1.00f);  // Frame
		const ImVec4 colBgHoverBase = ImVec4(0.18f, 0.19f, 0.22f, 1.00f);  // Hover base
		const ImVec4 colOutline = ImVec4(1.00f, 1.00f, 1.00f, 0.08f);

		const ImVec4 colText = ImVec4(0.93f, 0.96f, 0.98f, 1.00f);
		const ImVec4 colTextDisabled = ImVec4(colText.x, colText.y, colText.z, 0.40f);

		// Accent from HSV hue (easy to change: 0.55 ~ teal, 0.83 ~ purple, 0.03 ~ orange)
		const ImVec4 colAccent = HSV(accentHueHSV, 0.65f, 0.95f, 1.00f);
		const ImVec4 colAccentHover = HSV(accentHueHSV, 0.70f, 1.00f, 1.00f);
		const ImVec4 colAccentActive = HSV(accentHueHSV, 0.75f, 0.85f, 1.00f);
		const ImVec4 colAccentGhost = ImVec4(colAccent.x, colAccent.y, colAccent.z, 0.30f);

		// Clamp menu accent intensity to a safe range
		if (menuAccentIntensity < 0.0f) menuAccentIntensity = 0.0f;
		if (menuAccentIntensity > 0.6f) menuAccentIntensity = 0.6f;

		// Metrics / layout
		style->Alpha = 1.0f; style->DisabledAlpha = 0.6f;
		style->WindowPadding = ImVec2(10, 10);
		style->FramePadding = ImVec2(10, 6);
		style->ItemSpacing = ImVec2(10, 8);
		style->ItemInnerSpacing = ImVec2(6, 6);
		style->CellPadding = ImVec2(6, 6);

		style->WindowRounding = 8.0f;
		style->ChildRounding = 8.0f;
		style->FrameRounding = 6.0f;
		style->PopupRounding = 8.0f;
		style->TabRounding = 6.0f;
		style->GrabRounding = 6.0f;
		style->ScrollbarRounding = 8.0f;

		style->WindowBorderSize = 1.0f;
		style->FrameBorderSize = 1.0f;
		style->PopupBorderSize = 1.0f;
		style->TabBorderSize = 1.0f;

		style->WindowMinSize = ImVec2(200, 80);
		style->ScrollbarSize = 12.0f;
		style->GrabMinSize = 14.0f;

		style->IndentSpacing = 16.0f;
		style->ColumnsMinSpacing = 6.0f;

		style->WindowTitleAlign = ImVec2(0.5f, 0.5f);  // Centered titles
		style->ColorButtonPosition = ImGuiDir_Right;
		style->WindowMenuButtonPosition = ImGuiDir_None;

		// Colors
		ImVec4* colors = style->Colors;
		colors[ImGuiCol_Text] = colText;
		colors[ImGuiCol_TextDisabled] = colTextDisabled;

		// Window / child / popup
		colors[ImGuiCol_WindowBg] = colBgWindow;
		colors[ImGuiCol_ChildBg] = colBgBase;
		// Slight accent tint for popups so menus feel "purple-ish" without harming readability
		colors[ImGuiCol_PopupBg] = Lerp(colBgFrame, colAccent, 0.08f); colors[ImGuiCol_PopupBg].w = 0.98f;

		colors[ImGuiCol_Border] = colOutline;
		colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

		// Frames / inputs
		colors[ImGuiCol_FrameBg] = colBgFrame;
		colors[ImGuiCol_FrameBgHovered] = Lerp(colBgFrame, colAccent, 0.30f);
		colors[ImGuiCol_FrameBgActive] = Lerp(colBgFrame, colAccentActive, 0.50f);

		// Title bar & Menu bar — make the violet visible here
		colors[ImGuiCol_TitleBg] = Lerp(colBgWindow, colAccent, menuAccentIntensity * 0.35f);
		colors[ImGuiCol_TitleBgActive] = Lerp(colBgWindow, colAccent, menuAccentIntensity * 0.90f);
		colors[ImGuiCol_TitleBgCollapsed] = Lerp(colBgWindow, colAccent, menuAccentIntensity * 0.45f); colors[ImGuiCol_TitleBgCollapsed].w = 0.75f;

		// Menu bar background (also used by BeginMainMenuBar)
		colors[ImGuiCol_MenuBarBg] = Lerp(colBgWindow, colAccent, menuAccentIntensity);

		// Scrollbar
		colors[ImGuiCol_ScrollbarBg] = colBgWindow;
		colors[ImGuiCol_ScrollbarGrab] = colBgHoverBase;
		colors[ImGuiCol_ScrollbarGrabHovered] = Lerp(colBgHoverBase, colAccent, 0.45f);
		colors[ImGuiCol_ScrollbarGrabActive] = Lerp(colBgHoverBase, colAccentActive, 0.60f);

		// Checks / sliders / buttons
		colors[ImGuiCol_CheckMark] = colAccent;
		colors[ImGuiCol_SliderGrab] = colAccentGhost;
		colors[ImGuiCol_SliderGrabActive] = colAccentActive;

		colors[ImGuiCol_Button] = colBgFrame;
		colors[ImGuiCol_ButtonHovered] = Lerp(colBgFrame, colAccent, 0.55f);
		colors[ImGuiCol_ButtonActive] = Lerp(colBgFrame, colAccentActive, 0.75f);

		// Headers (used by Menu items: hover/active)
		colors[ImGuiCol_Header] = Lerp(colBgFrame, colAccent, 0.40f);
		colors[ImGuiCol_HeaderHovered] = Lerp(colBgFrame, colAccent, 0.65f);
		colors[ImGuiCol_HeaderActive] = Lerp(colBgFrame, colAccentActive, 0.85f);

		// Separators / grips
		colors[ImGuiCol_Separator] = colOutline;
		colors[ImGuiCol_SeparatorHovered] = Lerp(colOutline, colAccent, 0.60f);
		colors[ImGuiCol_SeparatorActive] = Lerp(colOutline, colAccentActive, 0.80f);

		colors[ImGuiCol_ResizeGrip] = ImVec4(1, 1, 1, 0.08f);
		colors[ImGuiCol_ResizeGripHovered] = Lerp(colBgHoverBase, colAccent, 0.55f);
		colors[ImGuiCol_ResizeGripActive] = Lerp(colBgHoverBase, colAccentActive, 0.75f);

		// Tabs
		colors[ImGuiCol_Tab] = Lerp(colBgFrame, colAccent, 0.35f);
		colors[ImGuiCol_TabHovered] = Lerp(colBgFrame, colAccentHover, 0.85f);
		colors[ImGuiCol_TabActive] = Lerp(colBgFrame, colAccentActive, 0.75f);
		colors[ImGuiCol_TabUnfocused] = Lerp(colBgFrame, colBgWindow, 0.30f);
		colors[ImGuiCol_TabUnfocusedActive] = Lerp(colBgFrame, colAccent, 0.55f);

		// Tables / plots / misc
		colors[ImGuiCol_TableHeaderBg] = Lerp(colBgFrame, colBgHoverBase, 0.35f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(1, 1, 1, 0.10f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(1, 1, 1, 0.06f);
		colors[ImGuiCol_TableRowBg] = ImVec4(1, 1, 1, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1, 1, 1, 0.03f);

		colors[ImGuiCol_TextSelectedBg] = ImVec4(colAccent.x, colAccent.y, colAccent.z, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(colAccent.x, colAccent.y, colAccent.z, 0.90f);

		colors[ImGuiCol_NavHighlight] = ImVec4(colAccent.x, colAccent.y, colAccent.z, 0.70f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1, 1, 1, 0.80f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0, 0, 0, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.45f);

		colors[ImGuiCol_PlotLines] = Lerp(colText, colAccent, 0.20f);
		colors[ImGuiCol_PlotLinesHovered] = colAccentHover;
		colors[ImGuiCol_PlotHistogram] = Lerp(colText, colAccent, 0.20f);
		colors[ImGuiCol_PlotHistogramHovered] = colAccentHover;

		// HiDPI: scale all size metrics if requested
		if (uiScale != 1.0f) style->ScaleAllSizes(uiScale);

		// cursor size
		style->MouseCursorScale = 1.f;
	}

	void ChangeStyleImGui(ImGuiStyle* ptrStyle)
	{
		ApplyNeoCarbonStyle(ptrStyle, 0.75f, 0.83f, 0.45f);
	}

	std::string AddressForImGui(uintptr_t address)
	{
		std::stringstream str;
		str << std::hex << address;

		std::string retStr = str.str();

		str.str("");
		str.clear();

		return retStr;
	}

	void PreThread()
	{
		HWND tWnd = FindWindowA("UnrealWindow", nullptr);
		if (tWnd)
		{
			CFG.gameHWND = tWnd;

			RECT TempRect;
			ZeroMemory(&TempRect, sizeof(RECT));
			GetClientRect(CFG.gameHWND, &TempRect);
			CFG.ScreenWidth = TempRect.right;
			CFG.ScreenHeight = TempRect.bottom;
		}
	}
}