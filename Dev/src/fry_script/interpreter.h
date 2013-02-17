#ifndef FRY_SCRIPT_INTERPRETER_H
#define FRY_SCRIPT_INTERPRETER_H

#include "interpreter_types.h"

namespace fry_script
{
	#define OP_ARGS bytecode_t** ByteCode, foundation::Array<u8>& Stack
	
	namespace interpreter
	{
		/// Runs the code provided, make sure to size-check it, it will just
		/// run enough commands
		bool Run( Interpreter& Interpreter, const bytecode_t* Code, u32 Size );

		/// @TODO: Check the performance-gain on inlining
		inline bool PushI( OP_ARGS );
		
		/// @TODO: Check the performance-gain on inlining
		inline bool AddI( OP_ARGS );

		/// @TODO: Check the performance-gain on inlining
		inline bool InvalidOp( OP_ARGS );
	}
}

#endif