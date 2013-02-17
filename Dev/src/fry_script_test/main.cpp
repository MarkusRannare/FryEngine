#include "interpretor_tests.h"
#include "token_generator_tests.h"

int main( int, char const *[] )
{
	memory_globals::init();

	int Result = UnitTest::RunAllTests();

	memory_globals::shutdown();

	return Result;
}