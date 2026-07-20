// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "DialogueExtensions.h"

#include "SampleDialogueConditions.generated.h"

/**
 * Shows a choice only when a Blackboard flag matches an expected value.
 *
 * Put a UDialogueEffect_SetFlag on an earlier node, then gate a later Choice option with this
 * condition to build simple state-dependent branches (e.g. "[Ask again]" only after "[Ask]").
 */
UCLASS(meta = (DisplayName = "Check Flag"))
class SIMPLEDIALOGUESAMPLE_API UDialogueCondition_CheckFlag : public UDialogueCondition
{
	GENERATED_BODY()

public:
	/** Blackboard key to read. A missing flag counts as false. */
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FName FlagName;

	/** The value FlagName must equal for this transition to be allowed. */
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	bool bExpected = true;

	virtual bool Evaluate_Implementation(UDialogueExecutionContext* Context) const override;
};
