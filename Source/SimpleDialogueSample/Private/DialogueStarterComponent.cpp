// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "DialogueStarterComponent.h"

#include "DialogueSubsystem.h"

#include "Engine/World.h"

UDialogueRunner* UDialogueStarterComponent::StartDialogue(APlayerController* Player)
{
	if (!Dialogue)
	{
		return nullptr;
	}

	const UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	if (UDialogueSubsystem* Subsystem = World->GetSubsystem<UDialogueSubsystem>())
	{
		// The owning actor is the speaker.
		return Subsystem->StartDialogue(Dialogue, Player, GetOwner());
	}

	return nullptr;
}
