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

// AkTypes.h

/// \file 
/// Data type definitions.

#pragma once

#define AK_LINUX_DESKTOP

#define AK_LINUX

#if defined(__LP64__) || defined(_LP64)
#ifdef __aarch64__
#define AK_CPU_ARM_64	
#else
#define AK_CPU_X86_64
#define AKSIMD_V4F32_SUPPORTED
#endif
#else
#ifdef __arm__
#define AK_CPU_ARM
#else
#define AK_CPU_X86 //x86
#define AKSIMD_V4F32_SUPPORTED
#endif
#endif

#if (defined AK_CPU_ARM || defined AK_CPU_ARM_64)
#define AK_CPU_ARM_NEON
#define AKSIMD_V4F32_SUPPORTED
#endif

#ifndef AK_LINUX_AOSP
#define AK_SUPPORT_WCHAR						///< Can support wchar
#define AK_ALSA
#define AK_PULSEAUDIO
#endif

#define AK_SUPPORT_THREADS

typedef void* AkStackTrace[ 64 ];

#include <AK/SoundEngine/Platforms/POSIX/AkTypes.h>
