#ifndef FRYCORE_MESSAGEBOX_H
#define FRYCORE_MESSAGEBOX_H

#include "Types.h"

namespace fry_core
{
	namespace message_box
	{
		enum EButton
		{
			B_AbortRetryIgnore = 0x00000002L,
			B_CancelTryAgainContinue = 0x00000006L,
			B_Help = 0x00004000L,
			B_Ok = 0x00000000L,
			B_OkCancel = 0x00000001L,
			B_RetryCancel = 0x00000005L,
			B_YesNo = 0x00000004L,
			B_YesNoCancel = 0x00000003L
		};

		enum EIcon
		{
			I_Exclamation = 0x00000030L,
			I_Warning = 0x00000030L,
			I_Information = 0x00000040L,
			I_Asterisk = 0x00000040L,
			I_Question = 0x00000020L,
			I_Stop = 0x00000010L,
			I_Error = 0x00000010L,
			I_Hand = 0x00000010L
		};

		enum EDefaultButton
		{
			DB_ButtonOne = 0x00000000L,
			DB_ButtonTwo = 0x00000100L,
			DB_ButtonThree = 0x00000200L,
			DB_ButtonFour = 0x00000300L
		};

		enum EModal
		{
			M_Application = 0x00000000L,
			M_System = 0x00001000L,
			M_Task = 0x00002000L
		};

		enum EButtonPressed
		{
			BP_Ok = 1,
			BP_Cancel = 2,
			BP_Abort = 3,
			BP_Retry = 4,
			BP_Ignore = 5,
			BP_Yes = 6,
			BP_No = 7,
			BP_TryAgain = 10,
			BP_Continue = 11
			
		};

		EButtonPressed Show( const char* Message, const char* Caption, u32 Type );
	};
}

#endif