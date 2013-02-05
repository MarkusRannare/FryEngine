#ifndef FRYCORE_STACK_H
#define FRYCORE_STACK_H

#include "array.h"

using namespace foundation;

namespace fry_core
{
	namespace stack
	{
		template<typename T>
		T* Increment( foundation::Array<T>& Array, u32 NumElements );

		template<typename T>
		T* Decrement( foundation::Array<T>& Array, u32 NumElements );
	}

	namespace stack
	{
		template<typename T>
		inline T* Increment( foundation::Array<T>& Array, u32 NumElements )
		{
			if( Array._size + NumElements > Array._capacity )
			{
				foundation::array::grow( Array );
			}
			
			T* PtrToEndOfStack = foundation::array::end( Array );

			Array._size += NumElements;

			return PtrToEndOfStack;
		}
		
		template<typename T>
		inline T* Decrement( foundation::Array<T>& Array, u32 NumElements )
		{
			Array._size -= NumElements;

			return foundation::array::end( Array );
		}
	}
}

#endif