// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "DialogueRunner.h" // for FOnDialogueNode / FOnDialogueEnded delegate types

#include "DialogueSubsystem.generated.h"

class AActor;
class APlayerController;
class UDialogueAsset;
class UDialogueRunner;

/**
 * A convenient, game-wide entry point for playing dialogues.
 *
 * This is the pattern most projects want: instead of every caller creating and wiring up its
 * own UDialogueRunner, they ask this subsystem to play a dialogue and bind their UI once to its
 * OnDialogueNode / OnDialogueEnded events. The subsystem owns a single runner and reuses it.
 *
 * Access it from anywhere with `GetWorld()->GetSubsystem<UDialogueSubsystem>()`
 * (or the "Get Dialogue Subsystem" node in Blueprint).
 */
UCLASS()
class SIMPLEDIALOGUESAMPLE_API UDialogueSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Start (or restart) a dialogue. Returns the runner driving it. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	UDialogueRunner* StartDialogue(UDialogueAsset* Asset, APlayerController* Player, AActor* Speaker);

	/** True while a dialogue is playing. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool IsDialogueActive() const;

	/** The shared runner, if one has been created yet. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	UDialogueRunner* GetActiveRunner() const { return ActiveRunner; }

	/** Bind your UI here — forwarded from the underlying runner. */
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueNode OnDialogueNode;

	/** Bind your UI here — forwarded from the underlying runner. */
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueEnded OnDialogueEnded;

private:
	// Forward the runner's events out through the subsystem so UI binds in one place.
	UFUNCTION()
	void HandleDialogueNode(const FText& Speaker, const FText& Text, const TArray<FDialogueOption>& Options);

	UFUNCTION()
	void HandleDialogueEnded();

	UPROPERTY()
	TObjectPtr<UDialogueRunner> ActiveRunner;
};
