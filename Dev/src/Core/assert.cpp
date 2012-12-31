#include "Assert.h"

#include "Debug.h"
#include "message_box.h"
#include "string_stream.h"

#include <cstdlib>

using namespace foundation;

namespace fry_core
{
	void Assert( const char* Expression, const char* File, int Line )
	{
		using namespace string_stream;

		Buffer B( foundation::memory_globals::default_allocator() );
		B << "Assertion failed: " << Expression << "\n\n" << File << ":" << Line;

		message_box::EButtonPressed ButtonPressed = message_box::Show( c_str( B ), "Assertion failed", message_box::B_CancelTryAgainContinue | message_box::I_Error );
		
		if( ButtonPressed == message_box::BP_Continue )
		{
			TOGGLE_BREAKPOINT();
			std::exit( -1 );
		}
		else if( ButtonPressed == message_box::BP_Cancel )
		{
			std::exit( -1 );
		}	
	}
}