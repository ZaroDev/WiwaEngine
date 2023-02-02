#include "AudioPanel.h"

#include "imgui.h"

#include <Wiwa/audio/Audio.h>
#include <Wiwa/Platform/Windows/WindowsPlatformUtils.h>

AudioPanel::AudioPanel(EditorLayer* instance) 
	: Panel("Audio", ICON_FK_HEADPHONES, instance)
{
}

AudioPanel::~AudioPanel()
{
}

void AudioPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);

	bool loaded_project = Audio::LoadedProject();

	if (loaded_project) {
		ImGui::TextColored(ImVec4(0, 255, 0, 1), "Loaded");
	}
	else {
		ImGui::TextColored(ImVec4(255, 252, 127, 1), "Not loaded");
	}

	ImGui::SameLine();
	if (ImGui::Button("Load project##audio")) {
		std::string filename = Wiwa::FileDialogs::OpenFile("Wwise bank (*.bnk)\0*.bnk\0");

		if (filename != "") {
			bool ret = Audio::LoadProject(filename.c_str());

			if (!ret) {
				WI_ERROR("Error loading project [{}]", Audio::GetLastError());
			}
		}
	}

	if (ImGui::CollapsingHeader("Banks##audio")) {
		const std::vector<Audio::BankData>& bank_list = Audio::GetLoadedBankList();

		size_t b_size = bank_list.size();

		uint32_t del_bank_id = Audio::INVALID_ID;

		for (size_t i = 0; i < b_size; i++) {
			ImGui::Text(bank_list[i].name.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Delete##audio")) {
				del_bank_id = i;
			}
		}

		if (del_bank_id != Audio::INVALID_ID) {
			Audio::UnloadBank(bank_list[del_bank_id].name.c_str());
		}

		if (ImGui::Button("Load bank##audio")) {
			std::string filename = Wiwa::FileDialogs::OpenFile("Wwise bank (*.bnk)\0*.bnk\0");

			if (filename != "") {
				bool ret = Audio::LoadBank(filename.c_str());

				if (!ret) {
					WI_ERROR("Error loading project [{}]", Audio::GetLastError());
				}
			}
		}
	}

	if (ImGui::CollapsingHeader("Events##audio")) {
		const std::vector<Audio::EventData>& event_list = Audio::GetLoadedEventList();

		size_t e_size = event_list.size();

		for (size_t i = 0; i < e_size; i++) {
			ImGui::Text(event_list[i].name.c_str());
			ImGui::SameLine();
			ImGui::PushID(i);
			if (ImGui::Button("Play")) {
				Audio::PostEvent(event_list[i].name.c_str());
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop")) {
				Audio::StopEvent(event_list[i].name.c_str());
			}
			ImGui::PopID();
		}

		if (ImGui::Button("Load event##audio")) {
			ImGui::OpenPopup("Load event##audio");
		}
	}

	if (ImGui::BeginPopup("Load event##audio")) {
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Load event").x) / 2.f);
		ImGui::Text("Load event");
		static char buffer[64] = { 0 };
		ImGui::Text("Event name");
		ImGui::SameLine();
		ImGui::InputText("##inputEvent", buffer, IM_ARRAYSIZE(buffer));

		if (ImGui::Button("Load##audio")) {
			bool ret = Audio::LoadEvent(buffer);

			if (!ret) {
				WI_ERROR("Error loading event [{}]", Audio::GetLastError());
			}

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel##audio")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::Button("Reload project##audio")) {
		bool ret = Audio::ReloadProject();

		if (!ret) {
			WI_ERROR("Error reloading audio [{}]", Audio::GetLastError());
		}
	}

	ImGui::End();
}
