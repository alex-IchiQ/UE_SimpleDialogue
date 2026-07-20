// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "SampleDialogueConditions.h"

#include "SampleDialogueTypes.h"
#include "DialogueExtensions.h"

#include "StructUtils/InstancedStruct.h"

bool UDialogueCondition_CheckFlag::Evaluate_Implementation(UDialogueExecutionContext* Context) const
{
	if (!Context)
	{
		return false;
	}

	// Read the flag; treat "never set" as false.
	bool bCurrent = false;
	if (const FInstancedStruct* Slot = Context->Blackboard.Find(FlagName))
	{
		if (const FDialogueFlag* Flag = Slot->GetPtr<FDialogueFlag>())
		{
			bCurrent = Flag->bValue;
		}
	}

	return bCurrent == bExpected;
}
