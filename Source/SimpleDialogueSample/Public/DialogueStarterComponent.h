// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "Components/ActorComponent.h"

#include "DialogueStarterComponent.generated.h"

class APlayerController;
class UDialogueAsset;
class UDialogueRunner;

/**
 * Drop this on an NPC (or any actor) to give it a dialogue.
 *
 * It holds the dialogue asset and, when asked, starts it through the UDialogueSubsystem with the
 * owning actor as the speaker. A typical setup: on interact, call StartDialogue(PlayerController).
 */
UCLASS(ClassGroup = (Dialogue), meta = (BlueprintSpawnableComponent))
class SIMPLEDIALOGUESAMPLE_API UDialogueStarterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** The conversation this actor offers. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TObjectPtr<UDialogueAsset> Dialogue;

	/** Start this actor's dialogue for the given player. Returns the runner, or null if nothing to play. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	UDialogueRunner* StartDialogue(APlayerController* Player);
};
