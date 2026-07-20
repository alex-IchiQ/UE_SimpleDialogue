// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNode_Root.generated.h"

UCLASS()
class SIMPLEDIALOGUEEDITOR_API UDialogueGraphNode_Root : public UEdGraphNode
{
	GENERATED_BODY()
	
public:
	virtual void AllocateDefaultPins() override;
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;

	virtual bool CanUserDeleteNode() const override { return false; }
	virtual bool CanDuplicateNode() const override { return false; }

	UEdGraphPin* GetOutputPin() const;
};
