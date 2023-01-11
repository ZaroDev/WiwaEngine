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
		void CompileWishader(const char* filename);
		void CompileFiles(const char* vertexShaderSource, const char* fragmentShaderSource, bool hasGS, std::string* geometryShaderSourceStr, bool& retflag);

		void LoadFromWiasset(const char* filename);

		uint32_t getID() { return m_IDprogram; }
		void Bind();
		void UnBind();
		void Delete();

		bool getAllOk() { return m_AllOk; }
		std::string* getFileData(const char* file);

		//Old shader functions required for 2D renderer
		unsigned int getUniformLocation(const char* uniform_name);

		template<class T> void setUniform(unsigned int uniform_id, T value);
		void setUniformInt(unsigned int uniform_id, int value);
		void setUniformUInt(unsigned int uniform_id, unsigned int value);
		void setUniformMat4(unsigned int uniform_id, glm::mat4 value);
		void setUniformFloat(unsigned int uniform_id, float value);
		void setUniformVec3(unsigned int uniform_id, glm::vec3 value);
		void setUniformVec4(unsigned int uniform_id, glm::vec4 value);

		inline std::vector<UniformField>& getUniforms() { return m_Uniforms; }
		// Dynamic shader functions
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

	template<>
	inline void Shader::setUniform<int>(unsigned int uniform_id, int value) {
		setUniformInt(uniform_id, value);
	}
	template<>
	inline void Shader::setUniform<unsigned int>(unsigned int uniform_id, unsigned int value) {
		setUniformUInt(uniform_id, value);
	}
	template<>
	inline void Shader::setUniform<glm::mat4>(unsigned int uniform_id, glm::mat4 value) {
		setUniformMat4(uniform_id, value);
	}

	template<>
	inline void Shader::setUniform<float>(unsigned int uniform_id, float value) {
		setUniformFloat(uniform_id, value);
	}
	template<>
	inline void Shader::setUniform<glm::vec2>(unsigned int uniform_id, glm::vec2 value) {

	}
	template<>
	inline void Shader::setUniform<glm::vec3>(unsigned int uniform_id, glm::vec3 value) {
		setUniformVec3(uniform_id, value);
	}

	template<>
	inline void Shader::setUniform<glm::vec4>(unsigned int uniform_id, glm::vec4 value) {
		setUniformVec4(uniform_id, value);
	}

	template<class T>
	inline void Shader::setUniform(unsigned int uniform_id, T value)
	{
		WI_CORE_ERROR("Setting uniform for a non existant specialization.");
	}
}
