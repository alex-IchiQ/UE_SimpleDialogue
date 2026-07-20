// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "UObject/Object.h"
#include "DialogueRunner.generated.h"

class AActor;
class APlayerController;

class UDialogueAsset;
class UDialogueNode;
class UDialogueExecutionContext;

/** One selectable branch handed to the UI. Pass `Index` back to UDialogueRunner::SelectOption. */
USTRUCT(BlueprintType)
struct FDialogueOption
{
	GENERATED_BODY()

	/** Text to display for this option. */
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FText Text;

	/** Opaque handle identifying the option; feed it straight back to SelectOption. */
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	int32 Index = 0;
};

/** Fired whenever a node is presented. `Options` is never empty (a line yields a single "Continue"). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueNode, const FText&, Speaker, const FText&, Text, const TArray<FDialogueOption>&, Options);

/** Fired once when the dialogue reaches a dead end or is otherwise stopped. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

/**
 * Drives playback of a compiled UDialogueAsset.
 *
 * Create one, bind OnDialogueNode / OnDialogueEnded to your UI, then call StartDialogue.
 * The runner walks the baked node graph: it presents the current node, waits for SelectOption,
 * runs OnEnter effects, evaluates choice conditions, and ends when there is nowhere left to go.
 */
UCLASS(BlueprintType)
class SIMPLEDIALOGUERUNTIME_API UDialogueRunner : public UObject
{
	GENERATED_BODY()

public:
	/** Begin playback from the asset's entry node. Safe to call again to restart. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(UDialogueAsset* Asset, APlayerController* Player, AActor* Speaker);

	/** Advance along the option with the given index (as handed out via OnDialogueNode). */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SelectOption(int32 OptionIndex);

	/** True while a dialogue is in progress. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool IsRunning() const { return CurrentNode != nullptr; }

protected:
	/** Move to `Node`, run its OnEnter effects, then present it. Null ends the dialogue. */
	void EnterNode(UDialogueNode* Node);

	/** Build the option list for the current node and broadcast OnDialogueNode. */
	void PresentCurrent();

	/** Clear state and broadcast OnDialogueEnded. */
	void Finish();

public:
	/** Bind your UI here to receive each presented node. */
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueNode OnDialogueNode;

	/** Bind your UI here to know when to close. */
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueEnded OnDialogueEnded;

protected:
	UPROPERTY()
	TObjectPtr<UDialogueAsset> CurrentAsset;

	UPROPERTY()
	TObjectPtr<UDialogueNode> CurrentNode;

	UPROPERTY()
	TObjectPtr<UDialogueExecutionContext> Context;

	/** Targets for the options last presented; parallel to their Index. Rebuilt every PresentCurrent. */
	UPROPERTY()
	TArray<TObjectPtr<UDialogueNode>> PendingTargets;
};
