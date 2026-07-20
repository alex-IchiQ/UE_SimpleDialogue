// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "SampleDialogueEffects.h"

#include "SampleDialogueTypes.h"
#include "DialogueExtensions.h"

#include "Engine/Engine.h"
#include "StructUtils/InstancedStruct.h"

void UDialogueEffect_Log::Execute_Implementation(UDialogueExecutionContext* Context)
{
	UE_LOG(LogTemp, Log, TEXT("[Dialogue] %s"), *Message);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, Message);
	}
}

void UDialogueEffect_SetFlag::Execute_Implementation(UDialogueExecutionContext* Context)
{
	if (!Context || FlagName.IsNone())
	{
		return;
	}

	// Wrap the bool in FDialogueFlag and store it under FlagName, replacing any previous value.
	FDialogueFlag Flag;
	Flag.bValue = bValue;
	Context->Blackboard.Add(FlagName, FInstancedStruct::Make(Flag));
}
