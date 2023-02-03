#pragma once

#include <Wiwa/core/Core.h>
#include <stdint.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/containers/List.h>
#include <glm/glm.hpp>

namespace Wiwa {
	struct DirectionalLight
	{
		glm::vec3 Direction;
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
	};
	struct PointLight
	{
		glm::vec3 Position;

		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;

		float Constant;
		float Linear;
		float Quadratic;
	};
	class WI_API FrameBuffer {
	private:
		uint32_t m_FBO;
		uint32_t m_ColorBufferTexture;
		uint32_t m_RBO;
		int m_Width, m_Height;

		bool m_Init;

		DirectionalLight m_Light;
		List<PointLight> m_PointLights;
	public:
		FrameBuffer();
		~FrameBuffer();

		void Init(int width, int height, bool depth=true);

		void Bind(bool clear=true);
		void Unbind();

		void Clear();

		inline int getWidth() { return m_Width; }
		inline int getHeight() { return m_Height; }

		inline uint32_t getColorBufferTexture() { return m_ColorBufferTexture; }
		inline uint32_t getDepthBufferTexture() { return m_RBO; }

		inline void setLight(const DirectionalLight& light) { m_Light = light; }
		inline void addPointLight(const PointLight& light) { m_PointLights.emplace_back(light); }

		inline DirectionalLight& getDirectionalLight() { return m_Light; }
		inline List<PointLight>* getPointLights() { return &m_PointLights; }
	};
}