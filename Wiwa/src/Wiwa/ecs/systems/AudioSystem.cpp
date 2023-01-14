#include <wipch.h>
#include "AudioSystem.h"
#include <Wiwa/audio/Audio.h>

namespace Wiwa {
	AudioSystem::AudioSystem()
	{

	}

	AudioSystem::~AudioSystem()
	{

	}

	void AudioSystem::OnAwake()
	{

	}

	void AudioSystem::OnInit()
	{
		Audio::RegisterGameObject(m_EntityId);
	}

	void AudioSystem::OnUpdate()
	{
		Transform3D* t3d = GetComponent<Transform3D>();
		AudioSource* asrc = GetComponent<AudioSource>();

		if (!asrc) return;

		if (!Audio::SetPosition(m_EntityId, t3d->position, Vector3F::FRONT, Vector3F::UP)) {
			WI_CORE_ERROR("Audio couldn't set position [{}]", Audio::GetLastError());
		}

		if (!asrc->posted) {
			if (!Audio::PostEvent("Ruido", m_EntityId)) {
				WI_CORE_ERROR("Audio couldn't post event [{}]", Audio::GetLastError());
			}

			asrc->posted = true;
		}

		// Execute audios
		WI_INFO("Audio");
	}

	void AudioSystem::OnDestroy()
	{
		Audio::UnregisterGameObject(m_EntityId);
	}
}