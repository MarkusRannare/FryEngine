#include "token_generator.h"
#include <core/array.h>
#include <core/memory.h>

using namespace foundation;

namespace fry_script
{
	/// Internal version of namespace, clients don't need to know this
	namespace token_generator
	{
		struct GeneratorState;

		typedef void (*StatChange)( GeneratorState& Generator, const char* Code, size_t Idx );
		typedef void (*ParsingFunction)( GeneratorState& Generator, const char* Code, size_t Idx );

		/// State for parsing tokens
		struct ParsingState
		{
			StatChange BeginState;
			StatChange EndState;

			ParsingFunction ParseToken;
		};

		enum EParsingState
		{
			PS_ParseWhitespace,
			PS_ParseToken,
			PS_DONE
		};

		

		void BeginParseWhitespace( GeneratorState& State, const char* Code, size_t Idx );
		void EndParseWhitespace( GeneratorState& State, const char* Code, size_t Idx );
		void ParseWhitespace( GeneratorState& State, const char* Code, size_t Idx );

		void BeginParseToken( GeneratorState& State, const char* Code, size_t Idx );
		void EndParseToken( GeneratorState& State, const char* Code, size_t Idx );
		void ParseToken( GeneratorState& State, const char* Code, size_t Idx );

		ParsingState TokenGenerationStates[] =
		{
			{ BeginParseWhitespace, EndParseWhitespace, ParseWhitespace },
			{ BeginParseToken, EndParseToken, ParseToken },
			{ NULL, NULL, NULL }
		};

		void ChangeState( GeneratorState& State, EParsingState NewState, const char* Code, size_t Idx );

		struct GeneratorState
		{
			/// No default constructor
			GeneratorState();

			GeneratorState( Array<GeneratedToken>& TokensToEmit ) :
				LastFoundIdx( 0 ),
				CurrentParsingState( &TokenGenerationStates[PS_ParseWhitespace] ),
				EmittedTokens( TokensToEmit )
			{
			}

			size_t LastFoundIdx;
			ParsingState* CurrentParsingState;
			Array<GeneratedToken>& EmittedTokens;
		};
	}

	

	namespace token_generator
	{
		void BeginParseWhitespace( GeneratorState& State, const char* Code, size_t Idx )
		{
		}

		void EndParseWhitespace( GeneratorState& State, const char* Code, size_t Idx )
		{
		}

		void ParseWhitespace( GeneratorState& State, const char* Code, size_t Idx )
		{
			if( !IsWhitespace( Code[Idx] ) )
			{
				ChangeState( State, PS_ParseToken, Code, Idx );
			}		}

		void BeginParseToken( GeneratorState& State, const char* Code, size_t Idx )
		{
			State.LastFoundIdx = Idx;
		}

		void EndParseToken( GeneratorState& State, const char* Code, size_t Idx )
		{
			GeneratedToken Token;

			Token.Length = Idx - State.LastFoundIdx;
			Token.Location = &Code[State.LastFoundIdx];

			array::push_back( State.EmittedTokens, Token );
		}

		void ParseToken( GeneratorState& State, const char* Code, size_t Idx )
		{
			if( IsWhitespace( Code[Idx] ) )
			{
				ChangeState( State, PS_ParseWhitespace, Code, Idx );
			}
		}

		void ChangeState( GeneratorState& State, EParsingState NewState, const char* Code, size_t Idx )
		{
			if( State.CurrentParsingState->EndState != NULL )
			{
				State.CurrentParsingState->EndState( State, Code, Idx );
			}
			State.CurrentParsingState = &TokenGenerationStates[NewState];
			if( State.CurrentParsingState->BeginState != NULL )
			{
				State.CurrentParsingState->BeginState( State, Code, Idx );
			}
		}

		bool GenerateTokens( const char* Code, Array<GeneratedToken>& out_Tokens )
		{
			GeneratorState State( out_Tokens );
			
			const size_t CodeLength = strlen( Code );
			size_t Idx = 0;
			for( ; Idx < CodeLength; ++Idx )
			{
				State.CurrentParsingState->ParseToken( State, Code, Idx );
			}
			ChangeState( State, PS_DONE, Code, Idx );

			return true;
		}

		bool GetNextToken( TokenGenerator* Generator, Array<GeneratedToken>& Tokens, const char** out_NextToken )
		{
			size_t CurrentIndex = Generator->_CurrentIndex++;
			if( CurrentIndex >= array::size( Tokens ) )
			{
				Generator->_CurrentIndex = -1;
				memory::mem_zero( Generator->_TempToken, sizeof( Generator->_TempToken ) );

				return false;
			}

			const GeneratedToken& CurrentToken = Tokens[CurrentIndex];

			memcpy_s( Generator->_TempToken, sizeof( Generator->_TempToken ), CurrentToken.Location, CurrentToken.Length );
			Generator->_TempToken[CurrentToken.Length] = '\0';
			*out_NextToken = Generator->_TempToken;

			return true;
		}

		bool IsWhitespace( char Token )
		{
			return Token == ' ' || Token == '\t' || Token == '\n' || Token == '\r';
		}
	}
}