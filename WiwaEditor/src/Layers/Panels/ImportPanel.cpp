#include "wipch.h"
#include "ImportPanel.h"
#include "../../Utils/EditorUtils.h"
#include <Wiwa/utilities/json/JSONDocument.h>
#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Image.h>

ImportPanel::ImportPanel(EditorLayer* instance)
	: Panel("Import", instance)
{
}

ImportPanel::~ImportPanel()
{
}

void ImportPanel::Draw()
{
	ImGui::Begin(name, &active);
	AssetContainer(m_CurrentPath.string().c_str());
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
        {
            const wchar_t* path = (const wchar_t*)payload->Data;
            std::wstring ws(path);
            std::string pathS(ws.begin(), ws.end());
            m_CurrentPath = pathS;   
            m_MetaPath = m_CurrentPath;
          
        }
        if (ModelExtensionComp(m_CurrentPath))
        {
            if (!m_MetaPath.empty())
            {
                Wiwa::ModelSettings settings;
                Wiwa::Resources::CreateMeta<Wiwa::Model>(m_CurrentPath.string().c_str(), &settings);
            }
            Wiwa::Resources::LoadMeta<Wiwa::Model>(m_CurrentPath.string().c_str(), &m_ModelSettings);
        }
        if (ImageExtensionComp(m_CurrentPath))
        {
            if (!m_MetaPath.empty())
            {
                Wiwa::ImageSettings settings;
                Wiwa::Resources::CreateMeta<Wiwa::Image>(m_CurrentPath.string().c_str(), &settings);
            }
            Wiwa::Resources::LoadMeta<Wiwa::Image>(m_CurrentPath.string().c_str(), &m_ImageSettings);
        }
        ImGui::EndDragDropTarget();
    }
    if (m_CurrentPath == "")
    {
        TextCentered("Drag and drop a file to import");
    }
    //Model import settings
    ModelImport();
    ImageImport();
	ImGui::End();
}

void ImportPanel::ImageImport()
{
    if (ImageExtensionComp(m_CurrentPath))
    {
        ImGui::Text("Image import settings");
        ImGui::Separator();
        if (ImGui::Button("Import"))
        {

        }
    }
}

void ImportPanel::ModelImport()
{
    if (ModelExtensionComp(m_CurrentPath))
    {
        ImGui::Text("Model import settings");
        ImGui::Separator();
        ImGui::Checkbox("Pre translated vertices", &m_ModelSettings.preTranslatedVertices);
        if (ImGui::Button("Import"))
        {
            Wiwa::JSONDocument doc;
            doc.AddMember("pre_translated_vertices", m_ModelSettings.preTranslatedVertices);
            doc.save_file(m_MetaPath.string().c_str());
        }
    }
}
