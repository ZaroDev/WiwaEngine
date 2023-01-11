#include <wipch.h>

#include "Shader.h"
#include <Wiwa/utilities/render/Material.h>

#include <fstream>
#include <string>
#include <sstream>
#include "Wiwa/core/Resources.h"
#include <glew.h>
#include <rapidjson.h>

namespace Wiwa {
	Shader::Shader()
	{
		m_AllOk = false;
		m_CompileState = State::ToCompile;
	}


	Shader::~Shader()
	{
	}

	void Shader::CreateDefault(const char* path)
	{
		std::string file = path;
		file += ".vs";

		std::ofstream vertexFile(file.c_str());
		vertexFile << 
			"#version 330 core\n\n// Positions/Coordinates\nlayout (location = 0) in vec3 aPos;\n// Colors\nlayout (location = 1) in vec3 aColor;\n// Texture Coordinates\nlayout (location = 2) in vec2 aTex;\n\n\n// Outputs the color for the Fragment Shader\nout vec3 color;\n// Outputs the texture coordinates to the fragment shader\nout vec2 texCoord;\n\n\n//Necesary to render the model DON'T touch\nuniform mat4 u_Model;\nuniform mat4 u_Proj;\nuniform mat4 u_View;\n\n\n\n\nvoid main()\n{\n\tgl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0);\n\n\t// Assigns the colors from the Vertex Data to \"color\"\n\tcolor = aColor;\n\t// Assigns the texture coordinates from the Vertex Data to \"texCoord\"\n\ttexCoord = aTex;\n}";
		vertexFile.close();
		
		file = path;
		file += ".fs";
		std::ofstream fragmentFile(file.c_str());
		fragmentFile <<
			"#version 330 core\n\n// Outputs colors in RGBA\nout vec4 FragColor;\n\n\n// Inputs the color from the Vertex Shader\nin vec3 color;\n// Inputs the texture coordinates from the Vertex Shader\nin vec2 texCoord;\n\n// Gets the Texture Unit from the main function\nuniform sampler2D u_Tex0;\n\n\nvoid main()\n{\n\tFragColor = texture(u_Tex0, texCoord);\n}";
		fragmentFile.close();

	}

	void Shader::Init(const char* filename)
	{
		Compile(filename);
	}

	void Shader::Compile(const char* filename)
	{
		std::string vertexFile = filename;
		vertexFile += ".vs";

		std::string fragmentFile = filename;
		fragmentFile += ".fs";

		std::string geometryFile = filename;
		geometryFile += ".gs";

		std::string* vertexShaderSourceStr = getFileData(vertexFile.c_str());

		if (!vertexShaderSourceStr) {
			std::string msg = "Couldn't open file: ";
			msg += vertexFile;
			WI_CORE_CRITICAL(msg.c_str());
			m_CompileState = State::Error;
			return;
		}

		const char* vertexShaderSource = vertexShaderSourceStr->c_str();

		std::string* fragmentShaderSourceStr = getFileData(fragmentFile.c_str());

		if (!fragmentShaderSourceStr) {
			std::string msg = "Couldn't open file: ";
			msg += fragmentFile;
			WI_CORE_CRITICAL(msg.c_str());
			m_CompileState = State::Error;
			return;
		}

		const char* fragmentShaderSource = fragmentShaderSourceStr->c_str();

		std::string* geometryShaderSourceStr = getFileData(geometryFile.c_str());
		bool hasGS = geometryShaderSourceStr;

		bool retflag;
		CompileFiles(vertexShaderSource, fragmentShaderSource, hasGS, geometryShaderSourceStr, retflag);
		if (retflag) return;

		delete vertexShaderSourceStr;
		delete fragmentShaderSourceStr;
		delete geometryShaderSourceStr;

		m_AllOk = true;
		m_Path = filename;
	}

	void Shader::CompileFiles(const char* vertexShaderSource, const char* fragmentShaderSource, bool hasGS, std::string* geometryShaderSourceStr, bool& retflag)
	{
		retflag = true;
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

			WI_CORE_CRITICAL(msg.c_str());
			m_CompileState = State::Error;
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

			WI_CORE_CRITICAL(msg.c_str());
			m_CompileState = State::Error;
			return;
		}

		m_IDprogram = glCreateProgram();
		glAttachShader(m_IDprogram, vertexShader);
		glAttachShader(m_IDprogram, fragmentShader);


		unsigned int geometryShader;
		if (hasGS)
		{
			const char* geometryShaderSource = geometryShaderSourceStr->c_str();
			geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
			glCompileShader(geometryShader);

			glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

			if (!success) {
				glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
				std::string msg = "Geometry shader compile error: ";
				msg += infoLog;

				WI_CORE_CRITICAL(msg.c_str());
				m_CompileState = State::Error;
				return;
			}
			glAttachShader(m_IDprogram, geometryShader);
		}

		glLinkProgram(m_IDprogram);

		glGetShaderiv(m_IDprogram, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(m_IDprogram, 512, NULL, infoLog);
			std::string msg = "Shader program compile error: ";
			msg += infoLog;

			WI_CORE_CRITICAL(msg.c_str());
			m_CompileState = State::Error;
			return;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (hasGS)
			glDeleteShader(geometryShader);
		retflag = false;

		m_CompileState = State::Compiled;

		m_Model = glGetUniformLocation(m_IDprogram, "u_Model");
		m_Proj = glGetUniformLocation(m_IDprogram, "u_Proj");
		m_View = glGetUniformLocation(m_IDprogram, "u_View");
	}

	void Shader::LoadFromWiasset(const char* filename)
	{
		std::string file_name = filename;
		file_name += ".wishader";
		JSONDocument shaderFile;

		if (!shaderFile.load_file(file_name.c_str()))
			return;
		std::string vertexShader;
		std::string fragmentShader;
		std::string geometryShader;

		bool hasGeometry = false;

		if (shaderFile.HasMember("vertex"))
			vertexShader = shaderFile["vertex"].get<const char*>();
		if (shaderFile.HasMember("fragment"))
			fragmentShader = shaderFile["fragment"].get<const char*>();
		if (shaderFile.HasMember("geometry"))
			geometryShader = shaderFile["geometry"].get<const char*>();
		
		if (!geometryShader.empty())
			hasGeometry = true;

		bool ret = false;
		CompileFiles(vertexShader.c_str(), fragmentShader.c_str(), hasGeometry, &geometryShader, ret);

		if (shaderFile.HasMember("uniforms"))
		{
			JSONValue uniforms = shaderFile["uniforms"];
			rapidjson::Value* val = uniforms.getValue();
			for (rapidjson::Value::MemberIterator p = val->MemberBegin(); p != val->MemberEnd(); ++p)
			{
				addUniform(p->name.GetString(), (UniformType)p->value.GetInt());
			}
		}
	}

	void Shader::Bind()
	{
		if (m_CompileState != State::Compiled)
			return;
		glUseProgram(m_IDprogram);
	}

	void Shader::UnBind()
	{
		glUseProgram(0);
	}

	void Shader::Delete()
	{
		glDeleteProgram(m_IDprogram);
	}

	std::string* Shader::getFileData(const char* file)
	{
		std::fstream shaderFile;

		shaderFile.open(file, std::ios::in);

		if (!shaderFile.is_open()) return NULL;

		std::stringstream buffer;

		buffer << shaderFile.rdbuf();

		shaderFile.close();

		return new std::string(buffer.str());
	}

	unsigned int Shader::getUniformLocation(const char* uniform_name)
	{
		return glGetUniformLocation(m_IDprogram, uniform_name);
	}

	void Shader::setUniformInt(unsigned int uniform_id, int value)
	{
		glUseProgram(m_IDprogram);
		glUniform1i(uniform_id, value);
	}

	void Shader::setUniformUInt(unsigned int uniform_id, unsigned int value)
	{
		glUseProgram(m_IDprogram);
		glUniform1ui(uniform_id, value);
	}

	void Shader::setUniformMat4(unsigned int uniform_id, glm::mat4 value)
	{
		glUseProgram(m_IDprogram);
		glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::setUniformFloat(unsigned int uniform_id, float value)
	{
		glUseProgram(m_IDprogram);
		glUniform1f(uniform_id, value);
	}

	void Shader::setUniformVec3(unsigned int uniform_id, glm::vec3 value)
	{
		glUseProgram(m_IDprogram);
		glUniform3f(uniform_id, value.x, value.y, value.z);
	}

	void Shader::setUniformVec4(unsigned int uniform_id, glm::vec4 value)
	{
		glUseProgram(m_IDprogram);
		glUniform4f(uniform_id, value.r, value.g, value.b, value.a);
	}

	void Shader::addUniform(const char* name, const UniformType type)
	{
		UniformField* uniform = getUniform(name);
		//Checking if we are
		if (uniform)
		{
			if (uniform->type == type)
				return;
			
			uniform->type = type;
			uniform->location = glGetUniformLocation(m_IDprogram, name);
			return;
		}
		UniformField field;
		field.name = name;
		field.type = type;
		field.location = glGetUniformLocation(m_IDprogram, name);


		m_Uniforms.emplace_back(field);
	}

	void Shader::deleteUniform(const char* name)
	{
		for (size_t i = 0; i < m_Uniforms.size(); i++)
		{
			if (m_Uniforms[i].name == name)
			{
				m_Uniforms.erase(m_Uniforms.begin() + i);
				i--;
			}
		}
	}

	void Shader::setUniformType(const char* name, const UniformType type)
	{
		UniformField* uniform = getUniform(name);
		if (!uniform)
		{
			WI_CORE_ERROR("Uniform name can't be find");
			return;
		}
		uniform->type = type;
	}

	void Shader::setUniformName(const char* oldName, const char* newName)
	{
		UniformField* uniform = getUniform(oldName);
		if (!uniform)
		{
			WI_CORE_ERROR("Uniform name can't be find in shader");
			return;
		}
		uniform->name = newName;
	}

	void Shader::SetMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj)
	{
		glUniformMatrix4fv(m_Model, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(m_View, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(m_Proj, 1, GL_FALSE, glm::value_ptr(proj));
	}

	Wiwa::UniformField* Shader::getUniform(const char* name)
	{
		if (m_Uniforms.empty())
			return nullptr;
		for (size_t i = 0; i < m_Uniforms.size(); i++)
		{
			if (m_Uniforms[i].name == name)
			{
				return &m_Uniforms[i];
			}
		}
		return nullptr;
	}

	void Shader::Save()
	{
		for (const auto& mat : m_MatRefs)
		{
			mat->Refresh();
		}
		Resources::Import<Shader>(m_Path.c_str(), this);
	}

}