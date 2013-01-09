#ifndef FRYCORE_VECTOR2_H
#define FRYCORE_VECTOR2_H

#ifdef WIN32
#pragma once
#endif // #ifdef WIN32

#include "Types.h"
#include "math_types.h"

namespace foundation
{
	FORCE_INLINE Vector2 operator +( const Vector2& lhs, const Vector2& rhs );

	Vector2 operator +( const Vector2& lhs, const Vector2& rhs )
	{
		Vector2 Temp;

		Temp.x = lhs.x + rhs.x;
		Temp.y = lhs.y + rhs.y;

		return Temp;
	}
}

#endif