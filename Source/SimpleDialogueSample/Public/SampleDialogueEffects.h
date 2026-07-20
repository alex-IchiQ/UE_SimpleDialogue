// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "DialogueExtensions.h"

#include "SampleDialogueEffects.generated.h"

/**
 * The "hello world" of effects: prints a message when its node is entered.
 * The simplest possible UDialogueEffect subclass — start here.
 */
UCLASS(meta = (DisplayName = "Log Message"))
class SIMPLEDIALOGUESAMPLE_API UDialogueEffect_Log : public UDialogueEffect
{
	GENERATED_BODY()

public:
	/** Text to print to the log and on-screen when this node is entered. */
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString Message = TEXT("Dialogue node entered");

	virtual void Execute_Implementation(UDialogueExecutionContext* Context) override;
};

/**
 * Writes a boolean flag into the run's Blackboard. Pair with UDialogueCondition_CheckFlag
 * to make a choice appear only after the player has done something earlier in the conversation.
 */
UCLASS(meta = (DisplayName = "Set Flag"))
class SIMPLEDIALOGUESAMPLE_API UDialogueEffect_SetFlag : public UDialogueEffect
{
	GENERATED_BODY()

public:
	/** Blackboard key to write. */
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FName FlagName;

	/** Value to store under FlagName. */
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	bool bValue = true;

	virtual void Execute_Implementation(UDialogueExecutionContext* Context) override;
};
