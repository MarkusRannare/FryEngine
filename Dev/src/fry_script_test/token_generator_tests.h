#include "UnitTest++.h"
#include <fry_script/token_generator.h>
#include <core/memory.h>

using namespace fry_script;
using namespace memory;

#include <iostream>

/// Helper that compares a char*[] with a array<GeneratedToken>, so that we can run tests easily
#define CHECK_ARRAY_STRING_EQUAL( EXPECTED_RESULT, ACTUAL_RESULT ) \
{ \
	const u32 EXPECTED_SIZE = sizeof( EXPECTED_RESULT ) / sizeof( EXPECTED_RESULT[0] ); \
	CHECK_EQUAL( EXPECTED_SIZE, array::size( ACTUAL_RESULT ) ); \
	if( EXPECTED_SIZE == array::size( ACTUAL_RESULT ) ) \
	{ \
		TokenGenerator Generator; \
		const char* NextToken = NULL; \
		size_t Idx = 0; \
		while( token_generator::GetNextToken( &Generator, ACTUAL_RESULT, &NextToken ) ) \
		{ \
			CHECK_EQUAL( 0, strcmp( NextToken, EXPECTED_RESULT[Idx++] ) ); \
		} \
	} \
}

struct TokenGeneratorCleanSetup
{
	TokenGeneratorCleanSetup() :
		GeneratedTokens( memory_globals::default_allocator() )
	{
	}

	~TokenGeneratorCleanSetup()
	{
		array::clear( GeneratedTokens );
	}

	Array<GeneratedToken> GeneratedTokens;
};

SUITE( TokenGenerator )
{
	TEST_FIXTURE( TokenGeneratorCleanSetup, EmptyString )
	{
		const char* Code = "";
		bool Result = token_generator::GenerateTokens( Code, GeneratedTokens );

		CHECK( Result );
		CHECK_EQUAL( (u32)0, array::size( GeneratedTokens ) );
	}

	TEST_FIXTURE( TokenGeneratorCleanSetup, SingleToken )
	{
		const char* Code = "SingleToken";
		const char* ExpectedResult[] = 
		{
			"SingleToken"
		};

		bool Result = token_generator::GenerateTokens( Code, GeneratedTokens );
	
		CHECK( Result );
		CHECK_ARRAY_STRING_EQUAL( ExpectedResult, GeneratedTokens );
	}

	TEST_FIXTURE( TokenGeneratorCleanSetup, TwoTokens )
	{
		const char* Code = "FirstToken SecondToken";
		const char* ExpectedResult[] = 
		{
			"FirstToken",
			"SecondToken"
		};

		bool Result = token_generator::GenerateTokens( Code, GeneratedTokens );

		CHECK( Result );
		CHECK_ARRAY_STRING_EQUAL( ExpectedResult, GeneratedTokens );
	}

	TEST_FIXTURE( TokenGeneratorCleanSetup, SpaceAtEnd )
	{
		const char* Code = "FirstToken SecondToken ";
		const char* ExpectedResult[] = 
		{
			"FirstToken",
			"SecondToken"
		};

		bool Result = token_generator::GenerateTokens( Code, GeneratedTokens );

		CHECK( Result );
		CHECK_ARRAY_STRING_EQUAL( ExpectedResult, GeneratedTokens );
	}
}