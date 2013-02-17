#include "interpreter.h"

#include <core/stack.h>
#include "stack_manipulation.h"

using namespace fry_core;

#include <core/Assert.h>

namespace fry_script
{
	namespace interpreter
	{
		bool Run( Interpreter& Interpreter, const bytecode_t* Code, u32 Size )
		{
			u8* InstructionPointer = (bytecode_t*)Code;
			
			while( InstructionPointer < Code + Size )
			{
				u8* CurrentOp = InstructionPointer++;
				
				switch( *CurrentOp )
				{
					case OC_PushI:
						PushI( &InstructionPointer, Interpreter._Stack );
						break;
					case OC_AddI:
						AddI( &InstructionPointer, Interpreter._Stack );
						break;
					default:
						InvalidOp( &InstructionPointer, Interpreter._Stack );
						break;
				}
			}
			
			return true;
		}

		bool PushI( OP_ARGS )
		{
			const size_t DataToConsume = sizeof(s32);
			const size_t ResultSize = sizeof(s32);
			
			u8* ReservedSpace = stack::Increment( Stack, ResultSize );

			memcpy_s( ReservedSpace, ResultSize, *ByteCode, DataToConsume );

			*ByteCode += DataToConsume;

			return true;
		}

		bool AddI( OP_ARGS )
		{
			GET_ITEM( Stack, s32, First );
			GET_ITEM( Stack, s32, Second );
		
			s32 Result = First + Second;

			u8* ResultData = stack::Increment( Stack, sizeof( s32 ) );

			memcpy_s( ResultData, sizeof(s32), &Result, sizeof(s32) );

			return true;
		}

		bool InvalidOp( OP_ARGS )
		{
			CHECK_ALWAYS( false && "Invalid OP-code" );

			return false;
		}
	}
}

