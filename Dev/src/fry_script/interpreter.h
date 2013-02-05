#ifndef FRY_SCRIPT_INTERPRETER_H
#define FRY_SCRIPT_INTERPRETER_H

#include "interpreter_types.h"

namespace fry_script
{
	namespace interpreter
	{
		/// Initializes a interpreter
		void Init( Interpreter& Interpreter );

		/// Deallocates the interpreter and cleanups all mess
		/// it has created
		void Shutdown( Interpreter& Interpreter );

		/// Runs the code provided, make sure to size-check it, it will just
		/// run enough commands
		bool Run( Interpreter& Interpreter, u8* Code, int Size );
	}
}

#endif