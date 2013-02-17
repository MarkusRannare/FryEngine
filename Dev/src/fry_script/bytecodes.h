#ifndef FRY_SCRIPT_BYTECODES_H
#define FRY_SCRIPT_BYTECODES_H

#include "core/Types.h"

typedef u8 opcode_t;
typedef u8 bytecode_t;

namespace fry_script
{
	enum EOPCodes
	{
#define OP_CODE(NAME, ID) OC_##NAME = ID,
#include "opcodes.h"
#undef OP_CODE			
	};
}

#endif