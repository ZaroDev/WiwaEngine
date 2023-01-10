#pragma once

#include <Wiwa/core/Core.h>

#include <Wiwa/utilities/math/Vector4f.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/math/Vector2i.h>

#include <glm/glm.hpp>
#include "shaders/Uniform.h"

namespace Wiwa {
	using ResourceId = size_t;
	class Shader;
	class WI_API Material
	{
	public:
		Material(Shader* shader = nullptr);
		Material(const char* file);
		~Material();

		void Bind();
		void UnBind();

		void Load(const char* path);
		void Save(const char* path);
		static void SaveMaterial(const char* path, Material* mat);
		static Material* LoadMaterial(const char* path);

		void Refresh();
		inline Shader* getShader() const { return m_Shader; }
		void setShader(Shader* shader, const char* path);
		inline const char* getMaterialPath() { return m_MaterialPath.c_str(); }
		inline const char* getShaderPath() { return m_ShaderPath.c_str(); }
		template<class T>
		void SetUniformData(const char* name, const T& value);
		Uniform* getUniform(const char* name);
		inline std::vector<Uniform>& getUniforms()
		{
			return m_Uniforms;
		}
		inline size_t getUniformIndex(const char* name)
		{
			for (size_t i = 0; i < m_Uniforms.size(); i++)
			{
				if (m_Uniforms[i].name == name)
				{
					return i;
				}
			}
			WI_CORE_ERROR("Can't find uniform with name {0} in material", name);
			return -1;
		}
	private:
		ResourceId m_ResourceId = 0;
		std::string m_MaterialPath;
		std::string m_ShaderPath;

		Shader* m_Shader = nullptr;
		std::vector<Uniform> m_Uniforms;
	};

	template<class T>
	void Wiwa::Material::SetUniformData(const char* name, const T& value)
	{
		Uniform* uniform = getUniform(name);
		if(!uniform)
		{
			WI_CORE_CRITICAL("Uniform name {0} doesn't exist in the current material", name);
			return;
		}
		uniform->setData(value, uniform->getType());
	}


}