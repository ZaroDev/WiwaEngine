#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>

#include <vector>
#include <string>

class WI_API Audio
{
public:
	struct EventData {
		std::string name;
		uint32_t hash;
	};

	struct BankData : public EventData {
		std::string path;
	};
private:
	// Private constructor = default
	Audio();

	// Set last error from result
	static void setLastError(int result);

	// =========== Variables ===========

	// Init bank path
	static std::string m_InitBankPath;

	// Last error message
	static std::string m_LastErrorMsg;

	// Loaded banks
	static std::vector<BankData> m_LoadedBanks;

	// Loaded events
	static std::vector<EventData> m_LoadedEvents;

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

	// Get loaded bank list
	static const std::vector<BankData>& GetLoadedBankList() { return m_LoadedBanks; }

	// Get loaded event list
	static const std::vector<EventData>& GetLoadedEventList() { return m_LoadedEvents; }

	// Check if a project has been loaded
	static bool LoadedProject() { return m_LoadedProject; }

	/* Load project init.bnk
	 * All banks are unloaded when loading a new project
	*/
	static bool LoadProject(const char* init_bnk);

	// Reload project, banks and events
	static bool ReloadProject();

	// Reload banks and events
	static bool ReloadBanks();

	// Reload events
	static bool ReloadEvents();

	// Load bank into audio engine with name
	static bool LoadBank(const char* bank);

	// Unload bank
	static bool UnloadBank(const char* bank);

	// Find event name in loaded events list
	static uint32_t FindEvent(const char* event_name);

	// Find bank name in loaded banks list
	static uint32_t FindBank(const char* bank_name);

	// Load event
	static bool LoadEvent(const char* event_name);

	// Unload event
	static bool UnloadEvent(const char* event_name);

	// Post an event into the audio engine for a specific gameobject
	static bool PostEvent(const char* event_name, uint64_t game_object);

	// Post an event into the default gameobject
	static bool PostEvent(const char* event_name) { return PostEvent(event_name, m_DefaultListener); }

	// Stop event for a specific gameobject
	static bool StopEvent(const char* event_name, uint64_t game_object);

	// Stop event for the default gameobject
	static bool StopEvent(const char* event_name) { return StopEvent(event_name, m_DefaultListener); }

	// Stop all events
	static bool StopAllEvents();

	// Register a gameobject
	static bool RegisterGameObject(uint64_t go_id);

	/* Unregister a gameobject
	* Also clears object from default listeners
	*/
	static bool UnregisterGameObject(uint64_t go_id);

	// Set gameobject's position and orientation
	static bool SetPositionAndOrientation(uint64_t go_id, const Wiwa::Vector3f& position, const Wiwa::Vector3f& front, const Wiwa::Vector3f& up);

	// Set gameobject's position
	static bool SetPosition(uint64_t go_id, const Wiwa::Vector3f& position);

	// Add default listener
	static bool AddDefaultListener(uint64_t go_id);

	// Unregister all gameobjects
	static bool UnregisterAllGameObjects();

	/* Unload all banks including init.bnk
	 * Also stops playing everything and removes all events and data associated to them
	*/
	static bool UnloadAllBanks();

	// Returns last error
	static const char* GetLastError() { return m_LastErrorMsg.c_str(); }

	// Returns project path
	static std::string GetProjectPath() { return m_InitBankPath; }

	static const uint32_t INVALID_ID = -1;
};