#include "UnitTest++.h"

#include <core/stack.h>
#include <fry_script/interpreter.h>
#include <fry_script/stack_manipulation.h>

using namespace foundation;
using namespace fry_core;
using namespace fry_script;

struct InterpreterCleanSetup
{
	InterpreterCleanSetup() :
		Interp( memory_globals::default_allocator() ),
		Bytecode( memory_globals::default_allocator() ),
		Stack( Interp._Stack )
	{
	}

	~InterpreterCleanSetup()
	{
	}

#pragma warning( disable: 4100 )
	InterpreterCleanSetup& operator=( const InterpreterCleanSetup& Other )
	{
		// Should never be here, but can't use CHECK as FRY_CORE::CHECK conflicts with C++TEST::CHECK
		return *this;
	}
#pragma warning( default: 4100 )

	Interpreter Interp;
	Array<u8> Bytecode;
	// Helper to prevent us from typing Interp._Stack all the time
	Array<u8>& Stack;
};

SUITE( InterpreterPushI )
{
	TEST_FIXTURE( InterpreterCleanSetup, PushI0 )
	{
		CHECK_EQUAL( (unsigned)0, array::size( Stack ) );

		ADD_CONSTANT( Bytecode, u8, (u8)fry_script::OC_PushI );
		ADD_CONSTANT( Bytecode, s32, 0 );

		bool Result = interpreter::Run( Interp, array::begin( Bytecode ), array::size( Bytecode ) );

		CHECK( Result );
		CHECK_EQUAL( (unsigned)4, array::size( Stack ) );
		GET_ITEM( Stack, s32, PushResult );
		CHECK_EQUAL( 0, PushResult );
	}

	TEST_FIXTURE( InterpreterCleanSetup, PushI666 )
	{
		CHECK_EQUAL( (unsigned)0, array::size( Stack ) );

		ADD_CONSTANT( Bytecode, u8, (u8)fry_script::OC_PushI );
		ADD_CONSTANT( Bytecode, s32, 666 );
		
		bool Result = interpreter::Run( Interp, array::begin( Bytecode ), array::size( Bytecode ) );

		CHECK( Result );
		CHECK_EQUAL( (unsigned)4, array::size( Stack ) );
		GET_ITEM( Stack, s32, PushResult );
		CHECK_EQUAL( 666, PushResult );
	}
}

SUITE( InterpreterAddI )
{
	TEST_FIXTURE( InterpreterCleanSetup, AddI00 )
	{
		ADD_CONSTANT( Bytecode, u8, (u8)fry_script::OC_PushI );
		ADD_CONSTANT( Bytecode, s32, 0 );
		ADD_CONSTANT( Bytecode, u8, (u8)fry_script::OC_PushI );
		ADD_CONSTANT( Bytecode, s32, 0 );
		ADD_CONSTANT( Bytecode, u8, (u8)fry_script::OC_AddI );

		bool Result = interpreter::Run( Interp, array::begin( Bytecode ), array::size( Bytecode ) );

		CHECK( Result );
		CHECK_EQUAL( (unsigned)4, array::size( Stack ) );
		GET_ITEM( Stack, s32, ResultValue );
		CHECK_EQUAL( 0, ResultValue );
	}

	TEST_FIXTURE( InterpreterCleanSetup, AddI42666 )
	{
		ADD_CONSTANT( Bytecode, u8, (u8)fry_script::OC_PushI );
		ADD_CONSTANT( Bytecode, s32, 42 );
		ADD_CONSTANT( Bytecode, u8, (u8)fry_script::OC_PushI );
		ADD_CONSTANT( Bytecode, s32, 666 );
		ADD_CONSTANT( Bytecode, u8, (u8)fry_script::OC_AddI );

		bool Result = interpreter::Run( Interp, array::begin( Bytecode ), array::size( Bytecode ) );

		CHECK( Result );
		CHECK_EQUAL( (unsigned)4, array::size( Stack ) );

		GET_ITEM( Stack, s32, ResultValue );
		CHECK_EQUAL( 708, ResultValue );
	}
}

int main( int, char const *[] )
{
	memory_globals::init();

	int Result = UnitTest::RunAllTests();

	memory_globals::shutdown();

	return Result;
}