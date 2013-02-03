#include "bytecode_generator.h"

#include "bytecodes.h"

namespace fry_script
{
	bool GenerateBytecode( const char* InputCode, bytecode_t** out_Bytecode, int& out_NumBytesCodes )
	{
		static bytecode_t Output[] = { PUSH, 0 };
		
		*out_Bytecode = Output;
		out_NumBytesCodes = 2;

		return true;
	}
}