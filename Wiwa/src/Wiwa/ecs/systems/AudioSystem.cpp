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

	void AudioSystem::OnEntityAdded(EntityId eid)
	{
		Audio::RegisterGameObject(eid);
	}

	void AudioSystem::OnEntityRemoved(EntityId eid)
	{
		Audio::UnregisterGameObject(eid);
	}

	void AudioSystem::OnUpdate(EntityId eid)
	{
		Transform3D* t3d = GetComponent<Transform3D>(eid);
		AudioSource* asrc = GetComponent<AudioSource>(eid);

		if (!asrc) return;

		if (!Audio::SetPosition(eid, t3d->position, Vector3F::FRONT, Vector3F::UP)) {
			WI_CORE_ERROR("Audio couldn't set position [{}]", Audio::GetLastError());
		}

		if (!asrc->posted) {
			if (!Audio::PostEvent("Ruido", eid)) {
				WI_CORE_ERROR("Audio couldn't post event [{}]", Audio::GetLastError());
			}

			asrc->posted = true;
		}

		// Execute audios
		WI_INFO("Audio");
	}
}