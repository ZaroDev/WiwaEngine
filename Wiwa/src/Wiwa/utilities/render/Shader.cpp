#include <wipch.h>

#include <Wiwa/utilities/render/Shader.h>

#include <fstream>
#include <string>
#include <sstream>

namespace Wiwa {
	Shader::Shader()
	{
		m_AllOk = false;
	}


	Shader::~Shader()
	{
	}

	void Shader::Init(const GLchar * filename)
	{
		std::string vertexFile = filename;
		vertexFile += ".vs";

		std::string fragmentFile = filename;
		fragmentFile += ".fs";

		std::string* vertexShaderSourceStr = getFileData(vertexFile.c_str());

		if (!vertexShaderSourceStr) {
			std::string msg = "Couldn't open file: ";
			msg += vertexFile;

			WI_CORE_ASSERT(msg.c_str());

			return;
		}

		const char* vertexShaderSource = vertexShaderSourceStr->c_str();

		std::string* fragmentShaderSourceStr = getFileData(fragmentFile.c_str());

		if (!fragmentShaderSourceStr) {
			std::string msg = "Couldn't open file: ";
			msg += fragmentFile;

			WI_CORE_ASSERT(msg.c_str());

			return;
		}

		const char* fragmentShaderSource = fragmentShaderSourceStr->c_str();

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);

			std::string msg = "Vertex shader compile error: ";
			msg += infoLog;

			WI_CORE_ASSERT(msg.c_str());

			return;
		}

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::string msg = "Fragment shader compile error: ";
			msg += infoLog;

			WI_CORE_ASSERT(msg.c_str());

			return;
		}

		m_IDprogram = glCreateProgram();
		glAttachShader(m_IDprogram, vertexShader);
		glAttachShader(m_IDprogram, fragmentShader);
		glLinkProgram(m_IDprogram);

		glGetShaderiv(m_IDprogram, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(m_IDprogram, 512, NULL, infoLog);
			std::string msg = "Shader program compile error: ";
			msg += infoLog;

			WI_CORE_ASSERT(msg.c_str());

			return;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		delete vertexShaderSourceStr;
		delete fragmentShaderSourceStr;

		m_AllOk = true;
	}

	unsigned int Shader::getUniformLocation(const char * uniform_name)
	{
		return glGetUniformLocation(m_IDprogram, uniform_name);
	}

	std::string* Shader::getFileData(const char * file)
	{
		std::fstream shaderFile;

		shaderFile.open(file, std::ios::in);

		if (!shaderFile.is_open()) return NULL;

		std::stringstream buffer;

		buffer << shaderFile.rdbuf();

		shaderFile.close();

		return new std::string(buffer.str());
	}
}