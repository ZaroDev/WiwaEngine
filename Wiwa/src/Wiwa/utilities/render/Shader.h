#pragma once

#include <glew.h>

#include <Wiwa/Log.h>

#include <string>
#include <typeinfo>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

namespace Wiwa {
	class WI_API Shader
	{
	public:
		Shader();
		~Shader();

		enum State {
			Vertex
		};

		void Init(const GLchar* filename);
		GLuint getID() { return m_IDprogram; }
		void Use() { glUseProgram(m_IDprogram); }
		void Delete() { glDeleteProgram(m_IDprogram); }

		bool getAllOk() { return m_AllOk; }

		unsigned int getUniformLocation(const char* uniform_name);

		template<class T> void setUniform(unsigned int uniform_id, T value);
	private:
		std::string* getFileData(const char* file);
		GLuint m_IDprogram;
		bool m_AllOk;
	};

	template<>
	inline void Shader::setUniform<glm::mat4>(unsigned int uniform_id, glm::mat4 value) {
		glUseProgram(m_IDprogram);
		glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(value));
	}

	template<>
	inline void Shader::setUniform<float>(unsigned int uniform_id, float value) {
		glUseProgram(m_IDprogram);
		glUniform1f(uniform_id, value);
	}

	template<>
	inline void Shader::setUniform<glm::vec3>(unsigned int uniform_id, glm::vec3 value) {
		glUseProgram(m_IDprogram);
		glUniform3f(uniform_id, value.x, value.y, value.z);
	}

	template<>
	inline void Shader::setUniform<glm::vec4>(unsigned int uniform_id, glm::vec4 value) {
		glUseProgram(m_IDprogram);
		glUniform4f(uniform_id, value.r, value.g, value.b, value.a);
	}

	template<class T>
	inline void Shader::setUniform(unsigned int uniform_id, T value)
	{
		WI_CORE_ASSERT("Setting uniform for a non existant specialization.");
	}
}
