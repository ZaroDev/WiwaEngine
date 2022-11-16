#pragma once

#include <imgui.h>
#include <string>

inline void TextCentered(const char* text) {
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 textSize = ImGui::CalcTextSize(text);

	ImGui::SetCursorPos({ (windowSize.x - textSize.x) * 0.5f, (windowSize.y - textSize.y) * 0.5f });
	ImGui::Text(text);
}

inline void RemoveWordFromLine(std::string& line, const std::string& word)
{
	auto n = line.find(word);
	if (n != std::string::npos)
	{
		line.erase(n, word.length());
	}
}

inline bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f)
{
	ImGuiStyle& style = ImGui::GetStyle();

	float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

	return ImGui::Button(label);
}

inline void AssetContainer(const char* label)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 20.0f);
	ImGui::InputText("##label", (char*)label, 64, ImGuiInputTextFlags_ReadOnly);
	ImGui::PopStyleVar();
}