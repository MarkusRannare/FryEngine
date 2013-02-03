#ifndef FRY_SCRIPT_BYTECODE_GENERATOR_H
#define FRY_SCRIPT_BYTECODE_GENERATOR_H

#include "bytecodes.h"

namespace fry_script
{
	bool GenerateBytecode( const char* InoutCode, bytecode_t** out_Bytecode, int& out_NumBytesCodes );
}

#endif