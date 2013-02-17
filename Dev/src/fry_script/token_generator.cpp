#include "token_generator.h"
#include <core/array.h>
#include <core/memory.h>

using namespace foundation;

static const char CharsToIgnore[] =
{
	// Whitespaces
	' ',
	'\t',
	'\n',
	'\r'
};

#include <iostream>

namespace fry_script
{
	namespace token_generator
	{
		bool GenerateTokens( const char* Code, Array<GeneratedToken>& out_Tokens )
		{
			GeneratedToken Tok;
			size_t LastFoundIdx = 0;

			Tok.Location = Code;
			Tok.Length = 0;

			size_t Idx = 0;
			for( ; Idx < strlen( Code ); ++Idx )
			{
				if( Code[Idx] == ' ' )
				{
					Tok.Length = Idx - LastFoundIdx;

					array::push_back( out_Tokens, Tok );

					LastFoundIdx = Idx;
					Tok.Location = &Code[Idx+1];
					Tok.Length = 0;
				}
			}

			Tok.Length = Idx - LastFoundIdx;
			if( Tok.Length > 0 )
			{
				array::push_back( out_Tokens, Tok );
			}

			return true;
		}

		bool GetNextToken( TokenGenerator* Generator, Array<GeneratedToken>& Tokens, const char** out_NextToken )
		{
			size_t CurrentIndex = Generator->CurrentIndex++;
			if( CurrentIndex >= array::size( Tokens ) )
			{
				Generator->CurrentIndex = -1;
				memory::mem_zero( Generator->TempToken, sizeof( Generator->TempToken ) );

				return false;
			}

			const GeneratedToken& CurrentToken = Tokens[CurrentIndex];

			memcpy_s( Generator->TempToken, sizeof( Generator->TempToken ), CurrentToken.Location, CurrentToken.Length );
			Generator->TempToken[CurrentToken.Length] = '\0';
			*out_NextToken = Generator->TempToken;

			return true;
		}
	}
}