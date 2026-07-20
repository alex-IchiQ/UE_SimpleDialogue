// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNode_Jump.generated.h"

/** Editor-only redirect: an input-only node that references a target node by its Tag.
 *  Lets branches loop back (e.g. to a Hub) without dragging a wire. Resolved at compile. */
UCLASS()
class SIMPLEDIALOGUEEDITOR_API UDialogueGraphNode_Jump : public UEdGraphNode
{
	GENERATED_BODY()
public:
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;

	UFUNCTION()
	TArray<FName> GetTargetTagOptions() const;
	
public:
	UPROPERTY(EditAnywhere, Category="Dialogue", meta=(GetOptions="GetTargetTagOptions"))
	FName TargetTag;
};
