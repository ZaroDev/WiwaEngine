#pragma once

namespace Wiwa {
	enum class UniformType
	{
		Bool = 0,
		Int,
		Uint,
		Float,
		fVec2,
		fVec3,
		fVec4,
		Mat2,
		Mat3,
		Mat4,
		Time,
		Sampler2D
	};
	//struct SamplerData
	//{
	//	uint32_t textureId;
	//	std::string path;

	//	SamplerData(){}
	//	SamplerData(uint32_t id, const char* _path)
	//		: textureId(id), path(_path){}
	//};
	class Uniform
	{
	public:
		Uniform() 
			: m_Type(UniformType::Bool), name(), m_Data(nullptr), m_UniformID(NULL)
		{}
		
		
		Uniform(std::string name, UniformType type)
			: m_Type(type), m_Data(nullptr), name(name), m_UniformID(NULL)
		{
			setEmptyData();
		}

		void sendToShader(const uint16_t shaderProgram, int& textureId);

		template <class T>
		void setData(const T& value, const UniformType type)
		{
			setType(type);

			//If it's already a value we need to free the data
			if (m_Data)
				free(m_Data);

			//Reserving memory and copy the value
			m_Data = malloc(sizeof(T));
			memcpy(m_Data, &value, sizeof(T));
		}

		void setEmptyData();

		template <class T> 
		void getData(T* data)
		{
			data = (T*)this->m_Data;
		}
		template <class T>
		T getData()
		{
			return *(T*)m_Data;
		}
		template <class T>
		T* getPtrData()
		{
			return (T*)m_Data;
		}
		inline UniformType getType() const { return m_Type; }
		inline void setType(UniformType val) { m_Type = val; }
	private:
		UniformType m_Type;
		void* m_Data;
		uint16_t m_UniformID;
	public:
		std::string name;
	};
}