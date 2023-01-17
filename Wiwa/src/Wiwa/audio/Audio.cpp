#include <wipch.h>
#include "Audio.h"
#include <iostream>

// AK Wwise
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>

#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include "samples/Win32/AkFilePackageLowLevelIOBlocking.h"

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>

#include <AK/SoundEngine/Common/AkTypes.h>

#include <Wiwa/utilities/filesystem/FileSystem.h>

uint32_t Audio::m_InitBank = 0;
std::vector<Audio::BankData> Audio::m_LoadedBanks;
std::vector<Audio::EventData> Audio::m_LoadedEvents;
bool Audio::m_LoadedProject = false;
std::string Audio::m_LastErrorMsg = "None";
std::string Audio::m_InitBankPath = "";
uint64_t Audio::m_DefaultListener = 0;

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

void cb(AkCallbackType type, AkCallbackInfo* info) {
    AkDurationCallbackInfo* e_info = (AkDurationCallbackInfo*)info;

    printf("Event id: %d\n", e_info->eventID);

    int* test = (int*)e_info->pCookie;
    printf("Cookie: %d\n", *test);
    printf("Time: %f\n", e_info->fDuration);
}

void Audio::setLastError(int result)
{
    AKRESULT akresult = (AKRESULT)result;

    switch (akresult)
    {
    case AK_NotImplemented:
        m_LastErrorMsg = "Not implemented";
        break;
    case AK_Success:
        m_LastErrorMsg = "Success";
        break;
    case AK_Fail:
        m_LastErrorMsg = "Fail";
        break;
    case AK_PartialSuccess:
        m_LastErrorMsg = "Partial success";
        break;
    case AK_NotCompatible:
        m_LastErrorMsg = "Not compatible";
        break;
    case AK_AlreadyConnected:
        m_LastErrorMsg = "Already connected";
        break;
    case AK_InvalidFile:
        m_LastErrorMsg = "Invalid file";
        break;
    case AK_AudioFileHeaderTooLarge:
        m_LastErrorMsg = "Audiofile header too large";
        break;
    case AK_MaxReached:
        m_LastErrorMsg = "Max reached";
        break;
    case AK_InvalidID:
        m_LastErrorMsg = "Invalid ID";
        break;
    case AK_IDNotFound:
        m_LastErrorMsg = "ID not found";
        break;
    case AK_InvalidInstanceID:
        m_LastErrorMsg = "Invalid instance ID";
        break;
    case AK_NoMoreData:
        m_LastErrorMsg = "No more data";
        break;
    case AK_InvalidStateGroup:
        m_LastErrorMsg = "Invalid state group";
        break;
    case AK_ChildAlreadyHasAParent:
        m_LastErrorMsg = "Child already has a parent";
        break;
    case AK_InvalidLanguage:
        m_LastErrorMsg = "Invalid language";
        break;
    case AK_CannotAddItseflAsAChild:
        m_LastErrorMsg = "Cannot add itself as a child";
        break;
    case AK_InvalidParameter:
        m_LastErrorMsg = "Invalid parameter";
        break;
    case AK_ElementAlreadyInList:
        m_LastErrorMsg = "Element already in list";
        break;
    case AK_PathNotFound:
        m_LastErrorMsg = "Path not found";
        break;
    case AK_PathNoVertices:
        m_LastErrorMsg = "Path no vertices";
        break;
    case AK_PathNotRunning:
        m_LastErrorMsg = "Path not running";
        break;
    case AK_PathNotPaused:
        m_LastErrorMsg = "Path not paused";
        break;
    case AK_PathNodeAlreadyInList:
        m_LastErrorMsg = "Pathnode already in list";
        break;
    case AK_PathNodeNotInList:
        m_LastErrorMsg = "Pathnode not in list";
        break;
    case AK_DataNeeded:
        m_LastErrorMsg = "Data needed";
        break;
    case AK_NoDataNeeded:
        m_LastErrorMsg = "No data needed";
        break;
    case AK_DataReady:
        m_LastErrorMsg = "Data ready";
        break;
    case AK_NoDataReady:
        m_LastErrorMsg = "No data ready";
        break;
    case AK_InsufficientMemory:
        m_LastErrorMsg = "Insufficient memory";
        break;
    case AK_Cancelled:
        m_LastErrorMsg = "Cancelled";
        break;
    case AK_UnknownBankID:
        m_LastErrorMsg = "Unknown bank ID";
        break;
    case AK_BankReadError:
        m_LastErrorMsg = "Bank read error";
        break;
    case AK_InvalidSwitchType:
        m_LastErrorMsg = "Invalid switch type";
        break;
    case AK_FormatNotReady:
        m_LastErrorMsg = "Format not ready";
        break;
    case AK_WrongBankVersion:
        m_LastErrorMsg = "Wrong bank version";
        break;
    case AK_FileNotFound:
        m_LastErrorMsg = "File not found";
        break;
    case AK_DeviceNotReady:
        m_LastErrorMsg = "Device not ready";
        break;
    case AK_BankAlreadyLoaded:
        m_LastErrorMsg = "Bank already loaded";
        break;
    case AK_RenderedFX:
        m_LastErrorMsg = "Rendered FX";
        break;
    case AK_ProcessNeeded:
        m_LastErrorMsg = "Process needed";
        break;
    case AK_ProcessDone:
        m_LastErrorMsg = "Process done";
        break;
    case AK_MemManagerNotInitialized:
        m_LastErrorMsg = "Memory manager not initialized";
        break;
    case AK_StreamMgrNotInitialized:
        m_LastErrorMsg = "Stream mgr not initialized";
        break;
    case AK_SSEInstructionsNotSupported:
        m_LastErrorMsg = "SSE instructions not supported";
        break;
    case AK_Busy:
        m_LastErrorMsg = "Busy";
        break;
    case AK_UnsupportedChannelConfig:
        m_LastErrorMsg = "Unsupported channel config";
        break;
    case AK_PluginMediaNotAvailable:
        m_LastErrorMsg = "Plugin media not available";
        break;
    case AK_MustBeVirtualized:
        m_LastErrorMsg = "Must be virtualized";
        break;
    case AK_CommandTooLarge:
        m_LastErrorMsg = "Command too large";
        break;
    case AK_RejectedByFilter:
        m_LastErrorMsg = "Rejected by filter";
        break;
    case AK_InvalidCustomPlatformName:
        m_LastErrorMsg = "Invalid custom platform name";
        break;
    case AK_DLLCannotLoad:
        m_LastErrorMsg = "DLL cannot load";
        break;
    case AK_DLLPathNotFound:
        m_LastErrorMsg = "DLL path not found";
        break;
    case AK_NoJavaVM:
        m_LastErrorMsg = "No java VM";
        break;
    case AK_OpenSLError:
        m_LastErrorMsg = "OpenSL error";
        break;
    case AK_PluginNotRegistered:
        m_LastErrorMsg = "Plugin not registered";
        break;
    case AK_DataAlignmentError:
        m_LastErrorMsg = "Data alignment error";
        break;
    case AK_DeviceNotCompatible:
        m_LastErrorMsg = "Device not compatible";
        break;
    case AK_DuplicateUniqueID:
        m_LastErrorMsg = "Duplicate unique ID";
        break;
    case AK_InitBankNotLoaded:
        m_LastErrorMsg = "Init bank not loaded";
        break;
    case AK_DeviceNotFound:
        m_LastErrorMsg = "Device not found";
        break;
    case AK_PlayingIDNotFound:
        m_LastErrorMsg = "Playing ID not found";
        break;
    case AK_InvalidFloatValue:
        m_LastErrorMsg = "Invalid float value";
        break;
    case AK_FileFormatMismatch:
        m_LastErrorMsg = "File format mismatch";
        break;
    case AK_NoDistinctListener:
        m_LastErrorMsg = "No distinct listener";
        break;
    case AK_ACP_Error:
        m_LastErrorMsg = "ACP error";
        break;
    case AK_ResourceInUse:
        m_LastErrorMsg = "Resource in use";
        break;
    case AK_InvalidBankType:
        m_LastErrorMsg = "Invalid bank type";
        break;
    case AK_AlreadyInitialized:
        m_LastErrorMsg = "Already initialized";
        break;
    case AK_NotInitialized:
        m_LastErrorMsg = "Not initialized";
        break;
    case AK_FilePermissionError:
        m_LastErrorMsg = "File permission error";
        break;
    case AK_UnknownFileError:
        m_LastErrorMsg = "Unknown file error";
        break;
    default:
        m_LastErrorMsg = "Unknown";
        break;
    }
}

bool Audio::Init()
{
    // Initialize memory manager
    AkMemSettings memSettings;
    AK::MemoryMgr::GetDefaultSettings(memSettings);

    if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
    {
        printf("Could not create the memory manager.");
        return false;
    }


    // Initialize stream manager
    AkStreamMgrSettings stmSettings;
    AK::StreamMgr::GetDefaultSettings(stmSettings);

    // Customize the Stream Manager settings here.


    if (!AK::StreamMgr::Create(stmSettings))
    {
        printf("Could not create the Streaming Manager");
        return false;
    }

    //

    // Create a streaming device with blocking low-level I/O handshaking.

    // Note that you can override the default low-level I/O module with your own. 

    //

    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

    // Customize the streaming device settings here.
    
    CAkDefaultIOHookBlocking lowLevelIO;

    // CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device

    // in the Stream Manager, and registers itself as the File Location Resolver.

    if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
    {
        printf("Could not create the streaming device and Low-Level I/O system");
        return false;
    }

    // Initialize sound engine
    AkInitSettings initSettings;
    AkPlatformInitSettings platformInitSettings;
    AK::SoundEngine::GetDefaultInitSettings(initSettings);
    AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

    if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
    {
        printf("Could not initialize the Sound Engine.");
        return false;
    }

    // Initialize music engine
    AkMusicSettings musicInit;
    AK::MusicEngine::GetDefaultInitSettings(musicInit);

    if (AK::MusicEngine::Init(&musicInit) != AK_Success)
    {
        printf("Could not initialize the Music Engine.");
        return false;
    }

    // Initialize spatial audio
    AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 

    if (AK::SpatialAudio::Init(settings) != AK_Success)
    {
        printf("Could not initialize the Spatial Audio.");
        return false;
    }

    m_DefaultListener = 20000;

    AKRESULT gres = AK::SoundEngine::RegisterGameObj(m_DefaultListener);

    if (gres != AK_Success) {
        setLastError(gres);
    }

    AK::SoundEngine::SetDefaultListeners(&m_DefaultListener, 1);
    //gres = AK::SoundEngine::AddDefaultListener(m_DefaultListener);

    if (gres != AK_Success) {
        setLastError(gres);
    }

    AkSoundPosition position;
    position.SetPosition(0.0f, 300.0f, 0.0f);
    position.SetOrientation(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

    gres = AK::SoundEngine::SetPosition(m_DefaultListener, position);

    if (gres != AK_Success) {
        setLastError(gres);
    }

    return true;
}

bool Audio::Update()
{
    AK::SoundEngine::RenderAudio();

    return true;
}

bool Audio::Terminate()
{
    //AK::SpatialAudio::Term();

    AK::MusicEngine::Term();
    AK::SoundEngine::Term();

    g_lowLevelIO.Term();

    if (AK::IAkStreamMgr::Get())
        AK::IAkStreamMgr::Get()->Destroy();

    AK::MemoryMgr::Term();

    return true;
}

bool Audio::LoadProject(const char* init_bnk)
{
    if (!UnloadAllBanks()) return false;

    AKRESULT res = AK::SoundEngine::LoadBank(init_bnk, m_InitBank);

    if (res != AK_Success) {
        setLastError(res);
        return false;
    }

    m_LoadedBanks.clear();
    m_LoadedEvents.clear();

    m_InitBankPath = init_bnk;
    m_LoadedProject = true;

    return true;
}

bool Audio::ReloadProject()
{
    if (!UnloadAllBanks()) return false;

    AKRESULT res = AK::SoundEngine::LoadBank(m_InitBankPath.c_str(), m_InitBank);

    if (res != AK_Success) {
        setLastError(res);
        return false;
    }

    ReloadBanks();

    m_LoadedProject = true;

    return true;
}

bool Audio::ReloadBanks() {
    size_t bnk_size = m_LoadedBanks.size();

    for (size_t i = 0; i < bnk_size; i++) {
        uint32_t id = 0;
        AKRESULT res = AK::SoundEngine::LoadBank(m_LoadedBanks[i].path.c_str(), id);

        if (res != AK_Success) {
            setLastError(res);

            // Remove from list
            m_LoadedBanks.erase(m_LoadedBanks.begin() + i);
            i--;
            bnk_size--;
        }
    }

    ReloadEvents();

    return true;
}

bool Audio::ReloadEvents() {
    size_t event_size = m_LoadedEvents.size();

    for (size_t i = 0; i < event_size; i++) {
        bool ret = PostEvent(m_LoadedEvents[i].name.c_str());

        if (!ret) {
            m_LoadedEvents.erase(m_LoadedEvents.begin() + i);
            i--;
            event_size--;
        }
        else {
            StopEvent(m_LoadedEvents[i].name.c_str());
        }
    }
    
    return true;
}

bool Audio::LoadBank(const char* bank)
{
    AkBankID bank_id;

    AKRESULT res = AK::SoundEngine::LoadBank(bank, bank_id);

    if (res != AK_Success) {
        setLastError(res);
        return false;
    }

    std::string filename = Wiwa::FileSystem::GetFileName(bank);

    m_LoadedBanks.emplace_back(BankData{ filename, bank_id, bank });

    return true;
}

bool Audio::UnloadBank(const char* bank)
{
    uint32_t b_id = FindBank(bank);

    if (b_id == INVALID_ID) return true;

    AKRESULT res = AK::SoundEngine::UnloadBank(m_LoadedBanks[b_id].path.c_str(), NULL);

    if (res != AK_Success) {
        setLastError(res);
        return false;
    }

    m_LoadedBanks.erase(m_LoadedBanks.begin() + b_id);

    ReloadEvents();

    return true;
}

uint32_t Audio::FindEvent(const char* event_name)
{
    uint32_t index = INVALID_ID;

    size_t event_size = m_LoadedEvents.size();

    for (size_t i = 0; i < event_size; i++) {
        if (m_LoadedEvents[i].name == event_name) {
            index = i;
            break;
        }
    }

    return index;
}

uint32_t Audio::FindBank(const char* bank_name)
{
    uint32_t index = INVALID_ID;

    size_t bank_size = m_LoadedBanks.size();

    for (size_t i = 0; i < bank_size; i++) {
        if (m_LoadedBanks[i].name == bank_name) {
            index = i;
            break;
        }
    }

    return index;
}

bool Audio::LoadEvent(const char* event_name)
{
    uint32_t ev_id = FindEvent(event_name);

    if (ev_id != INVALID_ID) return true;

    bool res = PostEvent(event_name);

    if (res != AK_Success) {
        setLastError(res);
        return false;
    }

    StopEvent(event_name);

    m_LoadedEvents.emplace_back(EventData{ event_name, AK::SoundEngine::GetIDFromString(event_name) });

    return true;
}

bool Audio::UnloadEvent(const char* event_name)
{
    uint32_t ev_id = FindEvent(event_name);

    if (ev_id == INVALID_ID) return true;

    m_LoadedEvents.erase(m_LoadedEvents.begin() + ev_id);

    return true;
}

bool Audio::PostEvent(const char* event_name, uint64_t game_object)
{
    AkPlayingID play_id = AK::SoundEngine::PostEvent(event_name, game_object);

    if (play_id == AK_INVALID_PLAYING_ID) {
        m_LastErrorMsg = "Couldn't post event [";
        m_LastErrorMsg += event_name;
        m_LastErrorMsg += "]";
        return false;
    }

    return true;
}

bool Audio::StopEvent(const char* event_name, uint64_t game_object)
{
    AKRESULT res = AK::SoundEngine::ExecuteActionOnEvent(event_name, AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Stop, game_object);

    if (res != AK_Success) {
        setLastError(res);
        return false;
    }

    return true;
}

bool Audio::StopAllEvents()
{
    AK::SoundEngine::StopAll();

    return true;
}

bool Audio::RegisterGameObject(uint64_t go_id)
{
    AKRESULT res = AK::SoundEngine::RegisterGameObj(go_id);

    if (res != AK_Success) {
        setLastError(res);
        return false;
    }

    return true;
}

bool Audio::UnregisterGameObject(uint64_t go_id)
{
    AKRESULT res = AK::SoundEngine::UnregisterGameObj(go_id);

    if (res != AK_Success) {
        setLastError(res);
        return false;
    }

    return true;
}

bool Audio::SetPositionAndOrientation(uint64_t go_id, const Wiwa::Vector3f& position, const Wiwa::Vector3f& front, const Wiwa::Vector3f& up)
{
    AkSoundPosition soundposition;
    soundposition.SetPosition(position.x, position.y, position.z);
    soundposition.SetOrientation(front.x, front.y, front.z, up.x, up.y, up.z);

    AKRESULT res = AK::SoundEngine::SetPosition(go_id, soundposition);

    if (res != AK_Success) {
        setLastError(res);
        return false;
    }

    return true;
}

bool Audio::SetPosition(uint64_t go_id, const Wiwa::Vector3f& position)
{
    AkSoundPosition soundposition;
    soundposition.SetPosition(position.x, position.y, position.z);

    AKRESULT res = AK::SoundEngine::SetPosition(go_id, soundposition);

    if (res != AK_Success) {
        setLastError(res);
        return false;
    }

    return false;
}

bool Audio::AddDefaultListener(uint64_t go_id)
{
    AKRESULT res = AK::SoundEngine::AddDefaultListener(go_id);

    if (res != AK_Success) {
        setLastError(res);
        return false;
    }

    return true;
}

bool Audio::UnregisterAllGameObjects()
{
    AKRESULT res = AK::SoundEngine::UnregisterAllGameObj();

    if (res != AK_Success) {
        setLastError(res);
        return false;
    }

    return true;
}

bool Audio::UnloadAllBanks()
{
    if (!m_LoadedProject) return true;

    AKRESULT res = AK::SoundEngine::ClearBanks();

    if (res != AK_Success) {
        setLastError(res);
        return false;
    }

    m_LoadedProject = false;

    return true;
}
