// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "UObject/Object.h"

#include "DialogueNode.generated.h"

class UDialogueNode;
class UDialogueCondition;
class UDialogueEffect;

/**
 * A single directed edge from one dialogue node to another.
 *
 * The graph editor bakes `TargetNode` at compile time (see FDialogueCompiler); designers
 * never set it by hand. `Condition` and `Label` are authored data that survive compilation.
 */
USTRUCT(BlueprintType)
struct FDialogueTransition
{
	GENERATED_BODY()

	/** Node this transition leads to. Baked from the graph wiring; null means "dead end". */
	UPROPERTY()
	TObjectPtr<UDialogueNode> TargetNode = nullptr;

	/** Optional gate. When set and it evaluates to false, the transition is hidden/skipped at runtime. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<UDialogueCondition> Condition;

	/** Player-facing text for this branch. Used by Choice nodes to label each option. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FText Label;
};

/**
 * Base class for every runtime dialogue node.
 *
 * Nodes are instanced data objects owned by a UDialogueAsset. Subclasses add the payload
 * (a line of text, a branching choice, ...); this base holds the wiring and side effects
 * common to all of them. Abstract on purpose — only concrete subclasses are placed in a graph.
 */
UCLASS(Abstract, EditInlineNew, BlueprintType)
class SIMPLEDIALOGUERUNTIME_API UDialogueNode : public UObject
{
	GENERATED_BODY()
public:
	UDialogueNode();

	/** Human-readable title shown on the graph node header. */
	virtual FText GetNodeTitle() const;

public:
	/** Stable identity, assigned on construction. Reserved for save games / external references. */
	UPROPERTY()
	FGuid Guid;

	/** Optional name used to target this node from a Jump node (see UDialogueGraphNode_Jump). */
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FName Tag;

	/** Outgoing transitions. Count and order mirror the node's output pins in the graph. */
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray<FDialogueTransition> Outputs;

	/** Effects run once, in order, the moment this node is entered (before it is presented). */
	UPROPERTY(EditAnywhere, Instanced, Category = "Dialogue")
	TArray<TObjectPtr<UDialogueEffect>> OnEnterEffects;
};

/** A single spoken line: one speaker, one block of text, and a single "continue" transition. */
UCLASS()
class SIMPLEDIALOGUERUNTIME_API UDialogueNode_Line : public UDialogueNode
{
	GENERATED_BODY()
public:
	virtual FText GetNodeTitle() const override;

	/** Name of who is speaking. Optional; empty falls back to a generic "Line" title. */
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FText SpeakerName;

	/** The line itself. */
	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = true))
	FText Text;
};

/**
 * A branching player choice. Carries no data of its own — every branch lives in `Outputs[].Label`.
 * It exists as a distinct class so the editor can give it its own color/shape and schema rules
 * (namely, multiple output pins).
 */
UCLASS()
class SIMPLEDIALOGUERUNTIME_API UDialogueNode_Choice : public UDialogueNode
{
	GENERATED_BODY()
public:
	virtual FText GetNodeTitle() const override;
};
