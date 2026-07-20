// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "UObject/Object.h"
#include "StructUtils/InstancedStruct.h"

#include "DialogueExtensions.generated.h"

class AActor;
class APlayerController;

class UDialogueRunner;

/**
 * Shared state passed to every Condition and Effect during playback.
 *
 * Holds the participants (player controller, speaker actor) and a Blackboard for arbitrary
 * per-run data. Lives as long as the owning UDialogueRunner's current dialogue.
 */
UCLASS(BlueprintType)
class SIMPLEDIALOGUERUNTIME_API UDialogueExecutionContext : public UObject
{
	GENERATED_BODY()
public:
	/** The runner playing this dialogue. */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDialogueRunner> Runner;

	/** The player the dialogue is shown to. Weak — the dialogue does not keep it alive. */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<APlayerController> Player;

	/** The actor speaking (NPC, etc.). Weak — may be destroyed mid-dialogue. */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> Speaker;

	/** Free-form scratch data for conditions/effects to read and write (quest flags, counters, ...). */
	UPROPERTY(BlueprintReadWrite)
	TMap<FName, FInstancedStruct> Blackboard;
};

/**
 * A gate on a transition. Override Evaluate to hide a choice unless some condition holds.
 * Blueprintable — subclass in C++ or Blueprint. Defaults to always-true.
 */
UCLASS(Abstract, EditInlineNew, Blueprintable, BlueprintType)
class SIMPLEDIALOGUERUNTIME_API UDialogueCondition : public UObject
{
	GENERATED_BODY()

public:
	/** Return true to allow the transition this condition guards. */
	UFUNCTION(BlueprintNativeEvent, Category = "Dialogue")
	bool Evaluate(UDialogueExecutionContext* Context) const;

	virtual bool Evaluate_Implementation(UDialogueExecutionContext* Context) const { return true; }
};

/**
 * A side effect. Override Execute to run game logic when a node is entered
 * (grant an item, set a flag, play a sound, ...). Blueprintable; defaults to a no-op.
 */
UCLASS(Abstract, EditInlineNew, Blueprintable, BlueprintType)
class SIMPLEDIALOGUERUNTIME_API UDialogueEffect : public UObject
{
	GENERATED_BODY()

public:
	/** Run the effect. Called once, when the owning node is entered. */
	UFUNCTION(BlueprintNativeEvent, Category = "Dialogue")
	void Execute(UDialogueExecutionContext* Context);

	virtual void Execute_Implementation(UDialogueExecutionContext* Context) {}
};
