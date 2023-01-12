#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>

#include <vector>
#include <string>

class WI_API Audio
{
private:
	// Private constructor = default
	Audio();

	// Set last error from result
	static void setLastError(int result);

	// =========== Variables ===========

	// Last error message
	static std::string m_LastErrorMsg;

	// Loaded banks
	static std::vector<uint32_t> m_LoadedBanks;

	// Determines if a project has been loaded
	static bool m_LoadedProject;

	// Project init.bank ID
	static uint32_t m_InitBank;

	// Default gameobject listener
	static uint64_t m_DefaultListener;
public:
	// Init audio engine
	static bool Init();

	// Update audio engine events
	static bool Update();

	// Terminate audio engine
	static bool Terminate();

	/* Load project init.bnk
	 * All banks are unloaded when loading a new project
	*/
	static bool LoadProject(const char* init_bnk);

	// Load bank into audio engine
	static bool LoadBank(const char* bank);

	// Unload bank
	static bool UnloadBank(const char* bank);

	// Load event
	static bool LoadEvent(const char* event_name);

	// Unload event
	static bool UnloadEvent(const char* event_name);

	// Post an event into the audio engine for a specific gameobject
	static bool PostEvent(const char* event_name, uint64_t game_object);

	// Register a gameobject
	static bool RegisterGameObject(uint64_t go_id);

	// Unregister a gameobject
	static bool UnregisterGameObject(uint64_t go_id);

	// Set gameobject's position
	static bool SetPosition(uint64_t go_id, const Wiwa::Vector3f& position, const Wiwa::Vector3f& front, const Wiwa::Vector3f& up);

	// Unregister all gameobjects
	static bool UnregisterAllGameObjects();

	/* Unload all banks including init.bnk
	 * Also stops playing everything and removes all events and data associated to them
	*/
	static bool UnloadAllBanks();

	// Returns last error
	static const char* GetLastError() { return m_LastErrorMsg.c_str(); }
};