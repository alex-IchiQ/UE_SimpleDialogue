// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "CoreMinimal.h"

#include "SampleDialogueTypes.generated.h"

/**
 * A trivial boolean payload stored in the execution context's Blackboard.
 *
 * The Blackboard is a `TMap<FName, FInstancedStruct>`, so any data you pass between
 * conditions and effects has to live inside a USTRUCT. This is the smallest useful one:
 * the SetFlag effect writes it, the CheckFlag condition reads it. Copy this pattern for
 * your own richer payloads (ints, names, structs of quest state, ...).
 */
USTRUCT(BlueprintType)
struct FDialogueFlag
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	bool bValue = false;
};
