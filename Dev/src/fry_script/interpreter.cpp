#include "interpreter.h"

#include <core/stack.h>

using namespace fry_core;

namespace fry_script
{
	namespace interpreter
	{
		void Init( Interpreter& Interpreter )
		{
			
		}

		void Shutdown( Interpreter& Interpreter )
		{
		}

		bool Run( Interpreter& Interpreter, u8* Code, int Size )
		{
			u8* ReservedSpace = stack::Increment( Interpreter._Stack, 4 );
			
			memcpy_s( ReservedSpace, 4, &Code[1], 4 );

			return true;
		}
	}
}