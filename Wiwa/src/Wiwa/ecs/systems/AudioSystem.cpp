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
		Audio::RegisterGameObject(m_EntityId);

		AudioSource* asrc = GetComponent<AudioSource>();

		if (!asrc) return;

		if(asrc->isDefaultListener){
			Audio::AddDefaultListener(m_EntityId);
		}
	}

	void AudioSystem::OnInit()
	{
		AudioSource* asrc = GetComponent<AudioSource>();

		if (!asrc) return;

		if (!Audio::PostEvent(asrc->eventName, m_EntityId)) {
			WI_CORE_ERROR("Audio couldn't post event [{}]", Audio::GetLastError());
		}
	}

	void AudioSystem::OnUpdate()
	{
		Transform3D* t3d = GetComponent<Transform3D>();		

		if (!Audio::SetPositionAndOrientation(m_EntityId, t3d->position, Vector3F::FRONT, Vector3F::UP)) {
			WI_CORE_ERROR("Audio couldn't set position [{}]", Audio::GetLastError());
		}
	}

	void AudioSystem::OnDestroy()
	{
		AudioSource* asrc = GetComponent<AudioSource>();

		if (!asrc) return;
		
		if (!Audio::StopEvent(asrc->eventName, m_EntityId)) {
			WI_CORE_ERROR("Audio couldn't stop event [{}]", Audio::GetLastError());
		}

		Audio::UnregisterGameObject(m_EntityId);
	}
}