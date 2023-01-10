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

#ifndef __AK_PLATFORM_CONTEXT_LINUX_H__
#define __AK_PLATFORM_CONTEXT_LINUX_H__

#include <AK/SoundEngine/Common/IAkPlatformContext.h>
#include <AK/SoundEngine/Common/AkTypes.h>

namespace AK
{
	/// Context specific to the Linux port of Wwise SDK.
	class IAkLinuxContext : public IAkPlatformContext
	{
	public:
		virtual bool UsePulseAudioServerInfo() = 0;
		virtual const char* GetStreamName(AkDeviceID deviceID = 0) = 0;
		virtual AkUInt32 GetChannelCount(AkDeviceID deviceID = 0) = 0;
		virtual void SetSinkInitialized(bool isInitialized) = 0;

		virtual bool IsPluginSupported(AkPluginID pluginID) = 0;
		virtual bool IsStreamReady(AkPluginID pluginID) = 0;
		virtual const char* GetStreamName(AkPluginID pluginID, AkDeviceID deviceID) = 0;
		virtual AkUInt32 GetChannelCount(AkPluginID pluginID, AkDeviceID deviceID) = 0;
		virtual void SetSinkInitialized(AkPluginID pluginID, bool isInitialized) = 0;
	};
}

#endif // __AK_PLATFORM_CONTEXT_LINUX_H__
