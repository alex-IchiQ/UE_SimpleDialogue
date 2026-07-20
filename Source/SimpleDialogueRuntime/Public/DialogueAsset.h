// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "UObject/Object.h"
#include "DialogueAsset.generated.h"

class UEdGraph;
class UDialogueNode;

/**
 * A saved dialogue: the flat list of runtime nodes plus the entry point.
 *
 * Designers edit the `Graph` (editor-only); on save the compiler bakes it down into `Nodes`
 * and `EntryNode`, which is all the runtime (UDialogueRunner) ever reads.
 */
UCLASS(BlueprintType)
class SIMPLEDIALOGUERUNTIME_API UDialogueAsset : public UObject
{
	GENERATED_BODY()

public:
	/** Every node in the dialogue. Rebuilt from the graph on each compile (orphans are dropped). */
	UPROPERTY(VisibleAnywhere, Instanced, Category = "Dialogue")
	TArray<TObjectPtr<UDialogueNode>> Nodes;

	/** Where playback begins. Resolved from the graph's Entry node on compile. */
	UPROPERTY(VisibleAnywhere, Category = "Dialogue")
	TObjectPtr<UDialogueNode> EntryNode;

#if WITH_EDITORONLY_DATA
	/** The authoring graph. Editor-only; never cooked into a build. */
	UPROPERTY()
	TObjectPtr<UEdGraph> Graph = nullptr;
#endif
};
