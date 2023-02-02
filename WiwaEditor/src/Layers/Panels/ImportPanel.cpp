#include "wipch.h"
#include "ImportPanel.h"
#include "../../Utils/EditorUtils.h"
#include <Wiwa/utilities/json/JSONDocument.h>
#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Image.h>

ImportPanel::ImportPanel(EditorLayer* instance)
	: Panel("Import", ICON_FK_FILE_ARCHIVE_O, instance)
{
}

ImportPanel::~ImportPanel()
{
}

void ImportPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);
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
            m_MetaPath += ".meta";
            if (ModelExtensionComp(m_CurrentPath))
            {
                if (!std::filesystem::exists(m_MetaPath))
                {
                    Wiwa::ModelSettings settings;
                    Wiwa::Resources::CreateMeta<Wiwa::Model>(m_CurrentPath.string().c_str(), &settings);
                }
                Wiwa::Resources::LoadMeta<Wiwa::Model>(m_CurrentPath.string().c_str(), &m_ModelSettings);
            }
            if (ImageExtensionComp(m_CurrentPath))
            {
                if (!std::filesystem::exists(m_MetaPath))
                {
                    Wiwa::ImageSettings settings;
                    Wiwa::Resources::CreateMeta<Wiwa::Image>(m_CurrentPath.string().c_str(), &settings);
                }
                Wiwa::Resources::LoadMeta<Wiwa::Image>(m_CurrentPath.string().c_str(), &m_ImageSettings);
            }
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
        const char* types[] = 
        {
            "None",
            "RGB DXT1",
            "RGBA DXT1",
            "RGBA DXT3",
            "RGBA DXT5"
        };

        static const char* currentItem;
        switch (m_ImageSettings.Compression)    
        {
        case Wiwa::CompressionType::NONE:
        {
            currentItem = types[0];
        }break;
        case Wiwa::CompressionType::RGB_DXT1:
        {
            currentItem = types[1];
        }break;
        case Wiwa::CompressionType::RGBA_DXT1:
        {
            currentItem = types[2];
        }break;
        case Wiwa::CompressionType::RGBA_DXT3:
        {
            currentItem = types[3];
        }break;
        case Wiwa::CompressionType::RGBA_DXT5:
        {
            currentItem = types[4];
        }break;
        }
        if (ImGui::BeginCombo("Target", currentItem))
        {
            for (int i = 0; i < 5; i++)
            {
                bool isSelected = (currentItem == types[i]);
                if (ImGui::Selectable(types[i], isSelected))
                {
                    currentItem = types[i];
                    m_ImageSettings.Compression = (Wiwa::CompressionType)i;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }
        ImGui::Checkbox("Interlaced", &m_ImageSettings.Interlaced);
        ImGui::InputInt("Offset X", &m_ImageSettings.OffsetX);
        ImGui::InputInt("Offset Y", &m_ImageSettings.OffsetY);
        ImGui::Checkbox("Mip mapping", &m_ImageSettings.MipMapping);
        ImGui::Checkbox("Anystropy", &m_ImageSettings.Anystropy);
        ImGui::Checkbox("Max anystropy", &m_ImageSettings.MaxAnystropy);
        ImGui::Checkbox("Blur gausian", &m_ImageSettings.BlurGausian);
        ImGui::Checkbox("Blur average", &m_ImageSettings.BlurAverage);
        ImGui::Checkbox("Contrast", &m_ImageSettings.Contrast);
        ImGui::InputInt("Amount of contrast", &m_ImageSettings.AmountOfContrast);
        ImGui::Checkbox("Alienify", &m_ImageSettings.Alienify);
        ImGui::Checkbox("Gamma correction", &m_ImageSettings.GammaCorrection);
        ImGui::Checkbox("Noise", &m_ImageSettings.Noise);
        ImGui::Checkbox("Equialize", &m_ImageSettings.Equialize);
        ImGui::Checkbox("Negative", &m_ImageSettings.Negative);
        ImGui::Checkbox("Pixelize", &m_ImageSettings.Pixelize);
        ImGui::Checkbox("Sharpen", &m_ImageSettings.Sharpen);
        ImGui::InputInt("Sharpen factor", &m_ImageSettings.SharpenFactor);
        ImGui::InputInt("Sharpen iterations", &m_ImageSettings.SharpenIterations);
        ImGui::Checkbox("Scale", &m_ImageSettings.Scale);
        if (ImGui::Button("Import"))
        {
            Wiwa::Resources::CreateMeta<Wiwa::Image>(m_CurrentPath.string().c_str(), &m_ImageSettings);
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
            Wiwa::Resources::CreateMeta<Wiwa::Model>(m_CurrentPath.string().c_str(), &m_ModelSettings);
            //TODO: Reimport the resource
        }
    }
}
