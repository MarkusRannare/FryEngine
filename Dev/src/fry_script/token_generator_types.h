#ifndef FRY_SCRIPT_TOKEN_GENERATOR_TYPES
#define FRY_SCRIPT_TOKEN_GENERATOR_TYPES

namespace fry_script
{
	/// Largest size of a allowed token
	const s32 MAX_TOKEN_SIZE = 255;

	/// Struct that describes the tokens generated from GenerateTokens
	/// It contains a pointer into the char* Code that is sent into the
	/// GenerateToken function
	struct GeneratedToken
	{
		const char* Location;
		u32	Length;
	};

	/// The state of a TokenGenerator (for multithreading support)
	struct TokenGenerator
	{
		TokenGenerator() :
			_CurrentIndex( 0 )
		{
		}

		/// When using GetNextToken, use this to know what the next token is
		int _CurrentIndex;
		/// Temporary buffer that holds the current token
		char _TempToken[MAX_TOKEN_SIZE];
	};
}

#endif