#ifndef FRY_SCRIPT_TOKEN_GENERATOR_H
#define FRY_SCRIPT_TOKEN_GENERATOR_H

#include <core/collection_types.h>
#include "token_generator_types.h"

namespace fry_script
{
	namespace token_generator
	{
		/// Generates tokens from code
		/// \param Code - the code to generator tokens from
		/// \param Allocator - the allocator that will allocate the tokens in out_Tokens
		/// \param out_Tokens - the tokes generated from Code
		bool GenerateTokens( const char* Code, foundation::Array<GeneratedToken>& out_Tokens );

		/// Copies a single token
		bool GetNextToken( TokenGenerator* Generator, foundation::Array<GeneratedToken>& Tokens, const char** out_NextToken );
	}
}

#endif