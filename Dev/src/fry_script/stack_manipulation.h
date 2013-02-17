#ifndef FRY_SCRIPT_STACK_MANIPILATION_H
#define FRY_SCRIPT_STACK_MANIPILATION_H

#include "core/stack.h"
#include <cstringt.h>

/// Creates a item called VAR in the local scope from the stack (and pops it)
#define GET_ITEM( STACK, TYPE, VAR ) \
	TYPE VAR; \
	foundation::memory::mem_zero( &VAR, sizeof( TYPE ) ); \
	{ \
		u8* PtrToVar = fry_core::stack::Decrement( STACK, sizeof( TYPE ) ); \
		memcpy_s( &VAR, sizeof( TYPE ), PtrToVar, sizeof( TYPE ) ); \
	}

/// Adds a item to the stack
// @NOTE: Can't be a constant
#define ADD_ITEM( STACK, VAR ) \
	{ \
		u8* ItemPtr = stack::Increment( STACK, sizeof( VAR ) ); \
		memcpy_s( ItemPtr, sizeof( VAR ), &VAR, sizeof( VAR ) ); \
	}

/// Adds a constant to the stack
#define ADD_CONSTANT( STACK, TYPE, VAR ) \
	{ \
		TYPE TmpVar = VAR; \
		ADD_ITEM( STACK, TmpVar ) \
	}

#endif