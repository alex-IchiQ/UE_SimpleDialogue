// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "DialogueSubsystem.h"

#include "DialogueRunner.h"

UDialogueRunner* UDialogueSubsystem::StartDialogue(UDialogueAsset* Asset, APlayerController* Player, AActor* Speaker)
{
	// Create the runner lazily and bind to it once; reuse it for every subsequent dialogue.
	if (!ActiveRunner)
	{
		ActiveRunner = NewObject<UDialogueRunner>(this);
		ActiveRunner->OnDialogueNode.AddDynamic(this, &UDialogueSubsystem::HandleDialogueNode);
		ActiveRunner->OnDialogueEnded.AddDynamic(this, &UDialogueSubsystem::HandleDialogueEnded);
	}

	ActiveRunner->StartDialogue(Asset, Player, Speaker);
	return ActiveRunner;
}

bool UDialogueSubsystem::IsDialogueActive() const
{
	return ActiveRunner && ActiveRunner->IsRunning();
}

void UDialogueSubsystem::HandleDialogueNode(const FText& Speaker, const FText& Text, const TArray<FDialogueOption>& Options)
{
	OnDialogueNode.Broadcast(Speaker, Text, Options);
}

void UDialogueSubsystem::HandleDialogueEnded()
{
	OnDialogueEnded.Broadcast();
}
