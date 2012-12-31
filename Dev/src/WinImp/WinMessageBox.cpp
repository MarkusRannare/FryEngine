#include <Core/message_box.h>

namespace fry_core
{
	namespace message_box
	{
		EButtonPressed Show( const char* Message, const char* Caption, u32 Type )
		{
			return (EButtonPressed)MessageBox( NULL, Message, Caption, Type );
		}
	}
}