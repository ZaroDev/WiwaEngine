#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/render/Image.h>
#include <Wiwa/utilities/render/shaders/Shader.h>

namespace Wiwa
{
	class WI_API Skybox 
	{
	public:
		Skybox();
		~Skybox();

		void LoadCubemap(const std::vector<const char*>& faces);
		uint32_t GetTextureID() { return m_TextureID; }

		class Material* m_Material;

		void Render();
	private:
		uint32_t m_TextureID;
		uint32_t VAO, VBO;
	};
}