#include "UnitTest++.h"

#include <core/stack.h>
#include <fry_script/interpreter.h>

using namespace foundation;
using namespace fry_core;
using namespace fry_script;

#define APPEND_INT( BYTECODE, INTEGER ) \
{ \
	s32 Integer = INTEGER; \
	u8* IntPtr = stack::Increment( BYTECODE, sizeof(s32) ); \
	memcpy_s( IntPtr, sizeof(s32), &Integer, sizeof(s32) ); \
}

#define APPEND_BYTE( BYTECODE, BYTE ) \
{ \
	array::push_back( BYTECODE, (u8)BYTE ); \
}

#define APPEND_OPCODE( BYTECODE, OP_CODE ) \
{ \
	array::push_back( BYTECODE, OP_CODE ); \
}

// TODO: Add helper functions to fetch parameters from the stack

struct InterpreterCleanSetup
{
	InterpreterCleanSetup() :
		Interp( memory_globals::default_allocator() ),
		Bytecode( memory_globals::default_allocator() )
	{
		interpreter::Init( Interp );
	}

	~InterpreterCleanSetup()
	{
		interpreter::Shutdown( Interp );
	}

	Interpreter Interp;
	Array<u8> Bytecode;
};


SUITE( InterpreterPushI )
{
	TEST_FIXTURE( InterpreterCleanSetup, PushI0 )
	{
		CHECK_EQUAL( (unsigned)0, array::size( Interp._Stack ) );

		APPEND_OPCODE( Bytecode, PushI );
		APPEND_INT( Bytecode, 0 );

		bool Result = interpreter::Run( Interp, array::begin( Bytecode ), array::size( Bytecode ) );

		CHECK( Result );
		CHECK_EQUAL( (unsigned)4, array::size( Interp._Stack ) );
		CHECK_EQUAL( 0, Interp._Stack[0] );
	}

	TEST_FIXTURE( InterpreterCleanSetup, PushI666 )
	{
		CHECK_EQUAL( (unsigned)0, array::size( Interp._Stack ) );

		APPEND_OPCODE( Bytecode, PushI );
		APPEND_INT( Bytecode, 666 );
		
		bool Result = interpreter::Run( Interp, array::begin( Bytecode ), array::size( Bytecode ) );

		CHECK( Result );
		CHECK_EQUAL( (unsigned)4, array::size( Interp._Stack ) );
		CHECK_ARRAY_EQUAL( &Bytecode[1], array::begin( Interp._Stack ), 4 );
	}
}

SUITE( InterpreterAddI )
{
	TEST_FIXTURE( InterpreterCleanSetup, AddI00 )
	{
		APPEND_OPCODE( Bytecode, PushI );
		APPEND_INT( Bytecode, 0 );
		APPEND_OPCODE( Bytecode, PushI );
		APPEND_INT( Bytecode, 0 );
		APPEND_OPCODE( Bytecode, AddI );

		bool Result = interpreter::Run( Interp, array::begin( Bytecode ), array::size( Bytecode ) );

		CHECK( Result );
		CHECK_EQUAL( (unsigned)4, array::size( Interp._Stack ) );
		CHECK_EQUAL( 0, Interp._Stack[0] );
	}

	TEST_FIXTURE( InterpreterCleanSetup, AddI42666 )
	{
		Array<u8> Bytecode( memory_globals::default_allocator() );
		APPEND_OPCODE( Bytecode, PushI );
		APPEND_INT( Bytecode, 42 );
		APPEND_OPCODE( Bytecode, PushI );
		APPEND_INT( Bytecode, 666 );
		APPEND_OPCODE( Bytecode, AddI );

		bool Result = interpreter::Run( Interp, array::begin( Bytecode ), array::size( Bytecode ) );

		s32 ResultOnStack = -1;
		memcpy_s( &ResultOnStack, sizeof(ResultOnStack), array::begin( Interp._Stack ), array::size( Interp._Stack ) );

		CHECK( Result );
		CHECK_EQUAL( (unsigned)4, array::size( Interp._Stack ) );
		CHECK_EQUAL( 708, ResultOnStack );
	}
}

int main( int, char const *[] )
{
	memory_globals::init();

	int Result = UnitTest::RunAllTests();

	memory_globals::shutdown();

	return Result;
}