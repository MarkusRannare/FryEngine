#include "UnitTest++.h"

#include <fry_script/fry_script.h>
#include <fry_script/bytecode_generator.h>

/*struct SomeFixture
{
	SomeFixture() { some setup }
	~SomeFixture() {some teardown }

	int testData;
};*/


SUITE( ByteCodeGenerator )
{
	TEST( Local0Int )
	{
		char* Code = "int hej = 0;";

		fry_script::Init();

		bytecode_t ExpectedOutput[] = { fry_script::PUSH, 0 };

		int NumByteCodes = 0;
		bytecode_t* Bytecode = NULL;
		bool Result = fry_script::GenerateBytecode( Code, &Bytecode, NumByteCodes );
		
		const int ExpectedOutputSize = sizeof( ExpectedOutput ) / sizeof( bytecode_t );
		CHECK_EQUAL( ExpectedOutputSize, NumByteCodes );
		CHECK( Result );
		CHECK_ARRAY_EQUAL( ExpectedOutput, Bytecode, ExpectedOutputSize );

		fry_script::Shutdown();
	}

	TEST( Local666Int )
	{
		char* Code = "int hej = 666;";

		fry_script::Init();

		bytecode_t ExpectedOutput[] = { fry_script::PUSH, 666 };

		int NumByteCodes = 0;
		bytecode_t* Bytecode = NULL;
		bool Result = fry_script::GenerateBytecode( Code, &Bytecode, NumByteCodes );

		const int ExpectedOutputSize = sizeof( ExpectedOutput ) / sizeof( bytecode_t );
		CHECK_EQUAL( ExpectedOutputSize, NumByteCodes );
		CHECK( Result );
		CHECK_ARRAY_EQUAL( ExpectedOutput, Bytecode, ExpectedOutputSize );

		fry_script::Shutdown();
	}

	/*TEST_FIXTURE(SomeFixture, YourTestName)
	{
		int temp = testData;
		temp = temp;
	}*/
}

int main( int, char const *[] )
{
	return UnitTest::RunAllTests();
}