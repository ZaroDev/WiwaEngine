#pragma once

#include <Wiwa/utilities/Log.h>

#include <string>
#include <typeinfo>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Uniform.h"
namespace Wiwa {
	class Material;
	struct UniformField
	{
		std::string name;
		UniformType type;
		uint32_t location;
	};

	class WI_API Shader
	{
	public:
		Shader();
		~Shader();

		enum State {
			ToCompile,
			Compiled,
			Error
		};

		static void CreateDefault(const char* path);

		void Init(const char* filename);
		void Compile(const char* filename);

		void CompileFiles(const char* vertexShaderSource, const char* fragmentShaderSource, bool hasGS, std::string* geometryShaderSourceStr, bool& retflag);

		void LoadFromWiasset(const char* filename);

		uint32_t getID() { return m_IDprogram; }
		void Bind();
		void UnBind();
		void Delete();

		bool getAllOk() { return m_AllOk; }
		std::string* getFileData(const char* file);

		inline std::vector<UniformField>& getUniforms() { return m_Uniforms; }

		void addUniform(const char* name, const UniformType type);
		void deleteUniform(const char* name);
		void setUniformType(const char* name, const UniformType type);
		void setUniformName(const char* oldName, const char* newName);
		void SetMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj);
		UniformField* getUniform(const char* name);
		
		inline const char* getPath() { return m_Path.c_str(); }
		inline void setPath(const char* path) { m_Path = path; }

		void Save();
		inline void AddRef(Material* ref)
		{
			m_MatRefs.emplace_back(ref);
		}
		void DeleteRef(Material* ref)
		{
			for (size_t i = 0; i < m_MatRefs.size(); i++)
			{
				if (m_MatRefs[i] == ref)
				{
					m_MatRefs.erase(m_MatRefs.begin() + i);
					break;
				}
			}
		}
		std::vector<Material*>& getRefs() { return m_MatRefs; }

		State getState() const { return m_CompileState; }
	protected:
		uint32_t m_IDprogram;
		bool m_AllOk;
		State m_CompileState;
		std::vector<UniformField> m_Uniforms;
		
		std::vector<Material*> m_MatRefs;
		
		std::string m_Path;


		uint32_t m_Model;
		uint32_t m_View;
		uint32_t m_Proj;
	};
}
