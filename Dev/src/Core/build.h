#ifndef FRYCORE_BUILD_H
#define FRYCORE_BUILD_H

#ifdef WIN32
#pragma once
#endif // #ifdef WIN32

/**
 * Specifies the games that are currently in development
 * with the engine.
 * 
 * defines CURRENTGAME with the game we want to compile
 * So after this it's valid to use #if CURRENTGAME == MYGAME to check
 * if you have special code for your game in the engine. Try to hold as many
 * of these checks in here to make sure the engine code don't get cluttered
 */
#include "GameIDS.h"

#define WINDOWS 0
/**
 * Defines compiler independent defines for each platform
 */
#ifdef WIN32
	#undef WINDOWS
	#define WINDOWS 1
#else
	#error The selected platform is not supported
#endif

#ifdef _DEBUG
	#define FRY_DEBUG 1
#else
	#define FRY_DEBUG 0
#endif

#ifdef NDEBUG
	#define FRY_RELEASE 1
#else
	#define FRY_RELEASE 0
#endif

#ifdef SHIPPING
	#define FRY_SHIPPING 1
#else
	#define FRY_SHIPPING 0
#endif

// Conditional expression is constant... I like constant expressions... 
#pragma warning( disable: 4127 ) 

// Remove unreferenced variable error in shipping, as they could come from variables used in log
// before
#if FRY_SHIPPING
	#pragma warning( disable : 4189 )
#endif

#endif