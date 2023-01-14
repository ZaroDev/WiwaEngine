#pragma once

#include <imgui.h>
#include <string>
#include <imgui_internal.h>
#include <glm/glm.hpp>
#include <filesystem>
#include <Wiwa/utilities/math/Vector3f.h>

struct Field;

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

void DrawVec3Control(const char* label, unsigned char* data, const Field field, float resetValue = 0.0f, float columnWidth = 100.0f);
void DrawVec3Control(const std::string& label,float* values, float resetValue = 0.0f, float columnWidth = 100.0f);
void DrawVec3Control(const std::string& label, Wiwa::Vector3f* values, float resetValue = 0.0f, float columnWidth = 100.0f);
void DrawVec2Control(const char* label, unsigned char* data, const Field field, float resetValue = 0.0f, float columnWidth = 100.0f);
void DrawInt2Control(const char* label, unsigned char* data, const Field field, int resetValue = 0.0f, float columnWidth = 100.0f);
void DrawRect2Control(const char* label, unsigned char* data, const Field field, int resetValue = 0.0f, float columnWidth = 100.0f);

bool ImageExtensionComp(const std::filesystem::path file);
bool ModelExtensionComp(const std::filesystem::path file);
bool ShaderExtensionComp(const std::filesystem::path file);
bool MaterialExtensionComp(const std::filesystem::path file);