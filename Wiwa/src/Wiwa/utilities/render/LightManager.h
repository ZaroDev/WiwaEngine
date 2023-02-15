#pragma once

#include <Wiwa/core/Core.h>

namespace Wiwa {
	class WI_API LightManager
	{
	public:

		LightManager();
		~LightManager();
		

		void AddLight(size_t light);
		void RemoveLight(size_t light);
		inline size_t GetLightsSize() { return m_Lights.size(); }
		inline std::vector<size_t>& GetLights() { return m_Lights; }

		inline void SetDirectionalLight(size_t light) { m_DirectionalLight = light; }
		inline size_t GetDirectionalLight() { return m_DirectionalLight; }

	private:
		std::vector<size_t> m_Lights;
		size_t m_DirectionalLight;
	};
}