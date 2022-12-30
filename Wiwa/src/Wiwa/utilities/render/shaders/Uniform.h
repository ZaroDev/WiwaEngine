#pragma once

namespace Wiwa {
	enum class UniformType
	{
		None = 0,
		Bool,
		Int,
		Uint,
		Float,
		Double,
		bVec2,
		bVec3,
		bVec4,
		iVec2,
		iVec3,
		iVec4,
		uVec2,
		uVec3,
		uVec4,
		fVec2,
		fVec3,
		fVec4,
		dVec2,
		dVec3,
		dVec4,
		Mat2,
		Mat3,
		Mat4,
		Mat2x2,
		Mat2x3,
		Mat2x4,
		Mat3x2,
		Mat3x3,
		Mat3x4,
		Mat4x2,
		Mat4x3,
		Mat4x4,
		Sampler,
		Sampler2D
	};

	class Uniform
	{
	public:
		UniformType type;

		bool isArray;
		int arraySize;
		void* data;

		Uniform(UniformType _type)
			: type(_type) {}

		template <class T> void getData(T* data)
		{
			data = (T*)this->data;
		}
		template <class T> T getData()
		{
			return *(T*)data;
		}
		template <class T> T* getPtrData()
		{
			return (T*)data;
		}
	};
}