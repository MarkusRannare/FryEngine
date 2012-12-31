#ifndef FRYCORE_ASSERT_H
#define FRYCORE_ASSERT_H

#include "Build.h"

namespace fry_core
{
	void Assert( const char* Expression, const char* File, int Line );
}

#define CHECK_ALWAYS( EXPRESSION ) \
	do \
	{ \
		if( !(EXPRESSION) ) \
		{ \
			fry_core::Assert( #EXPRESSION, __FILE__, __LINE__); \
		} \
	}while( 0 )

#ifdef FRY_DEBUG
	#define CHECK_DEBUG( EXPRESSION ) CHECK_ALWAYS( EXPRESSION )
	#define CHECK( EXPRESSION ) CHECK_ALWAYS( EXPRESSION )
#elif FRY_RELEASE
	#define CHECK_DEBUG( EXPRESSION ) ((void)0)
	#define CHECK( EXPRESSION ) CHECK_ALWAYS( EXPRESSION )
#elif FRY_SHIPPING
	#define CHECK_DEBUG( EXPRESSION ) ((void)0)
	#define CHECK( EXPRESSION ) ((void)0)
#endif

#endif