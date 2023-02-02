#include "AboutPanel.h"

#include "imgui.h"

#include <Wiwa/core/Application.h>

AboutPanel::AboutPanel(EditorLayer* instance) 
	: Panel("About", ICON_FK_INFO, instance)
{
}

AboutPanel::~AboutPanel()
{
}

void AboutPanel::Draw()
{
	Wiwa::SysInfo& info = Wiwa::Application::Get().GetSystemInfo();

	ImGui::Begin(iconName.c_str(), &active);
	ImGui::Separator();
	ImGui::Text("Wiwa Engine version: %s", Wiwa::Application::Get().GetVersion());
	ImGui::Text("Our attempt at doing a 3D Game engine");
	ImGui::Text("By Victor Falcon & Pablo Llorente");
	ImGui::Separator();
	ImGui::Text("3rd party libraries used:");
	ImGui::BulletText("GLFW v%s", info.glfwVer);
	ImGui::BulletText("Glew v7.0");
	ImGui::BulletText("ImGui v1.89");
	ImGui::BulletText("GLM v0.9.9.8");
	ImGui::BulletText("PCG v0.9");
	ImGui::BulletText("PugiXML v1.6");
	ImGui::BulletText("RapiJson v1.1.0");
	ImGui::BulletText("Spdlog v1.10.0");
	ImGui::BulletText("Optick v1.4.0");
	ImGui::BulletText("Assimp v5.2.5");
	ImGui::BulletText("ImGuizmo v1.89");
	ImGui::BulletText("Stb Image v2.27");
	ImGui::Separator();
	ImGui::Text("MIT License\n");
	ImGui::Text("Copyright (c) 2022 Xymaru & ZaroDev");
	ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
	ImGui::Text("of this software and associated documentation files (the 'Software'), to deal");
	ImGui::Text("in the Software without restriction, including without limitation the rights");
	ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell");
	ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
	ImGui::Text("furnished to do so, subject to the following conditions:");
	ImGui::Text("The above copyright notice and this permission notice shall be included in all");
	ImGui::Text("copies or substantial portions of the Software.");
	ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
	ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
	ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE");
	ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
	ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
	ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
	ImGui::Text("SOFTWARE.");
	ImGui::End();
}
