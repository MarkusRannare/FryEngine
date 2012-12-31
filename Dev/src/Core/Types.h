#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include "Build.h"

#if WINDOWS
	#pragma once
	#include <Windows.h>
#endif	// #ifdef WIN32

// Platform independent types

#ifndef NULL
	#define NULL (void*)0
#endif // #ifndef NULL

#define INDEX_NONE -1

#ifdef WINDOWS
	#define PATH_SEPARATOR '\\'
#else
	#error Pathseparator not defined
#endif

/*#ifndef FALSE
	#define FALSE 0
	#define TRUE !FALSE
#else
	#undef FALSE
	#define FALSE 0
	#ifdef TRUE
		#undef TRUE
	#endif
	#define TRUE !FALSE
#endif*/

// Windows (MSVS specific) types
#ifdef _MSC_VER
	typedef __int64				s64;
	typedef unsigned __int64	u64;

	typedef __int32				s32;
	typedef unsigned __int32	u32;

	typedef __int16				s16;
	typedef unsigned __int16	u16;

	typedef __int8				s8;
	typedef unsigned __int8		u8;
	
	//@todo: Check if there is a more correct alias for this
	typedef double				d64;
#else	// #ifdef _MSC_VER
	#error Basic types (u32, u8 a.s.o not defined for the selected platform)
#endif

#ifdef WIN32
	typedef u32 PTR_TYPE;
#elif defined WIN64
	typedef u64 PTR_TYPE;
#else
	#error PTR_TYPE not implemented for the current platform
#endif

#ifdef WIN32
	#define FORCE_INLINE __forceinline
#else
	#error Undefined platform, FORCE_INLINE undefined
#endif
// Just make sure we only use FORCE_INLINE to preserve
// platform independent code
#ifdef FORCEINLINE
	#undef FORCEINLINE
#endif

#ifdef WIN32
	#define THREAD_FUNC WINAPI
#else
	#error Threading is not yet implemented for the current platform
#endif

// Platform independent aliases

// Should this be added? Some are more comfortable with it,
// but a standard would be good too
//typedef u8				BYTE;

#ifdef WIN32
	#define UNUSED( x ) (void*)x
#else
#error UNUSED is unspecified for the current platform
#endif // #ifdef WIN32

// <BitsquidFoundation>
#include <stdint.h>

// Can't use this in C++0x, as alignof is a reserved keyword there
#include <xkeycheck.h>
#ifndef alignof
	#define alignof(x) __alignof(x)
#endif
// </BitsquidFoundation>

#endif	// #ifndef CORE_TYPES_H