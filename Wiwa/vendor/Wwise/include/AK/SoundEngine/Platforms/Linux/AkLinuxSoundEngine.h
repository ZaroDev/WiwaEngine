/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the 
"Apache License"); you may not use this file except in compliance with the 
Apache License. You may obtain a copy of the Apache License at 
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2022 Audiokinetic Inc.
*******************************************************************************/

// AkLinuxSoundEngine.h

/// \file 
/// Main Sound Engine interface, specific to Linux.

#pragma once

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>

/// \cond !(Web)
///< API used for audio output
///< Use with AkPlatformInitSettings to select the API used for audio output.  
///< Use AkAPI_Default, it will select the more appropriate API depending on the computer's capabilities.  Other values should be used for testing purposes.
///< \sa AK::SoundEngine::Init
typedef enum AkAudioAPILinux
{
	AkAPI_PulseAudio = 1 << 0,						///< Use PulseAudio (this is the preferred API on Linux)
	AkAPI_ALSA = 1 << 1,							///< Use ALSA
	AkAPI_Default = AkAPI_PulseAudio | AkAPI_ALSA,	///< Default value, will select the more appropriate API
} AkAudioAPI;

/// Platform specific initialization settings
/// \sa AK::SoundEngine::Init
/// \sa AK::SoundEngine::GetDefaultPlatformInitSettings
struct AkPlatformInitSettings
{
	// Threading model.
	AkThreadProperties	threadLEngine;				///< Lower engine threading properties
	AkThreadProperties  threadOutputMgr;			///< Ouput thread threading properties
	AkThreadProperties  threadBankManager;			///< Bank manager threading properties (its default priority is AK_THREAD_PRIORITY_NORMAL)
	AkThreadProperties  threadMonitor;				///< Monitor threading properties (its default priority is AK_THREAD_PRIORITY_ABOVENORMAL). This parameter is not used in Release build.
	
	//Voices.
	AkUInt32			uSampleRate;				///< Sampling Rate. Default 48000 Hz
	AkUInt16			uNumRefillsInVoice;			///< Number of refill buffers in voice buffer. 2 == double-buffered, defaults to 4.
	AkAudioAPI			eAudioAPI;					///< Main audio API to use. Leave to AkAPI_Default for the default sink (default value).
														///< If a valid audioDeviceShareset plug-in is provided, the AkAudioAPI will be Ignored.
														///< \ref AkAudioAPI
	AkDataTypeID		sampleType;					///< Sample type. AK_FLOAT for 32 bit float, AK_INT for 16 bit signed integer, defaults to AK_FLOAT.
													///< Supported by AkAPI_PulseAudio only.
};
/// \endcond
