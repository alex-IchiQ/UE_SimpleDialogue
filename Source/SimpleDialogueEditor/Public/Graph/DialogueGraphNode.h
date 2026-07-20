// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNode.generated.h"

class UDialogueNode;

UCLASS()
class SIMPLEDIALOGUEEDITOR_API UDialogueGraphNode : public UEdGraphNode
{
	GENERATED_BODY()
public:
	virtual void ReconstructNode() override;
	virtual void AllocateDefaultPins() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	
	virtual FText GetNodeTitle(ENodeTitleType::Type) const override;
	virtual FLinearColor GetNodeTitleColor() const override;

	void SetInstance(UDialogueNode* Node) { NodeInstance = Node; }
	UDialogueNode* GetInstance() const { return NodeInstance; }

protected:
	UPROPERTY()
	TObjectPtr<UDialogueNode> NodeInstance;
};
