#ifndef FRY_SCRIPT_TOKEN_GENERATOR_TYPES
#define FRY_SCRIPT_TOKEN_GENERATOR_TYPES

namespace fry_script
{
	/// Largest size of a allowed token
	const s32 MaxTokenSize = 255;

	/// Struct that describes the tokens generated from GenerateTokens
	/// It contains a pointer into the char* Code that is sent into the
	/// GenerateToken function
	struct GeneratedToken
	{
		const char* Location;
		u32	Length;
	};

	/// The state of a TokenGenerator (for multithread support)
	struct TokenGenerator
	{
		TokenGenerator() :
			CurrentIndex( 0 )
		{
		}

		int CurrentIndex;
		char TempToken[MaxTokenSize];
	};
}

#endif