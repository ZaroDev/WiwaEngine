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

		// Execute audios

	}
}