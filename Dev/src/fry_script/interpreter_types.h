#ifndef FRY_SCRIPT_INTERPRETER_TYPES_H
#define FRY_SCRIPT_INTERPRETER_TYPES_H

#include <core/collection_types.h>
#include "bytecodes.h"

namespace fry_script
{
	struct Interpreter
	{
		///
		Interpreter( foundation::Allocator& Alloc ) :
			_Stack( Alloc )
		{
		}

		foundation::Array<u8> _Stack;
	};
}

#endif