#pragma once

#include <Wiwa/utilities/Log.h>

#include <string>
#include <typeinfo>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Wiwa {
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

		void Init(const char* filename);
		void Compile(const char* filename);
		uint32_t getID() { return m_IDprogram; }
		void Bind();
		void UnBind();
		void Delete();

		bool getAllOk() { return m_AllOk; }

		unsigned int getUniformLocation(const char* uniform_name);

		template<class T> void setUniform(unsigned int uniform_id, T value);

		void setUniformInt(unsigned int uniform_id, int value);
		void setUniformUInt(unsigned int uniform_id, unsigned int value);
		void setUniformMat4(unsigned int uniform_id, glm::mat4 value);
		void setUniformFloat(unsigned int uniform_id, float value);
		void setUniformVec3(unsigned int uniform_id, glm::vec3 value);
		void setUniformVec4(unsigned int uniform_id, glm::vec4 value);
		std::string* getFileData(const char* file);
	private:
		uint32_t m_IDprogram;
		bool m_AllOk;
		State m_CompileState;
	};
	template<>
	inline void Shader::setUniform<int>(unsigned int uniform_id, int value) {
		setUniformInt(uniform_id, value);
	}
	template<>
	inline void Shader::setUniform<unsigned int>(unsigned int uniform_id, unsigned int value){
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
	inline void Shader::setUniform<glm::vec2>(unsigned int uniform_id, glm::vec2 value){

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
