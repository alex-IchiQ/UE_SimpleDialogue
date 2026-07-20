// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "EdGraph/EdGraphSchema.h"

class UDialogueNode;

struct FDialogueGraphSchemaAction_NewNode : FEdGraphSchemaAction
{
	static FName StaticGetTypeId() { static FName Type(TEXT("FDialogueGraphSchemaAction_NewNode")); return Type; }

public:
	FDialogueGraphSchemaAction_NewNode() = default;
	FDialogueGraphSchemaAction_NewNode(TSubclassOf<UDialogueNode> InNodeClass, FText InCategory, FText InMenuDesc, FText InTooltip);
	
	virtual FName GetTypeId() const override { return StaticGetTypeId(); }
	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override;

public:
	TSubclassOf<UDialogueNode> NodeClass;
};

struct FDialogueGraphSchemaAction_NewJump : public FEdGraphSchemaAction
{
	FDialogueGraphSchemaAction_NewJump(FText Category, FText MenuDesc, FText Tooltip);
	
	static FName StaticGetTypeId() { static FName Type(TEXT("FDialogueGraphSchemaAction_NewJump")); return Type; }
	
	virtual FName GetTypeId() const override { return StaticGetTypeId(); }
	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override;
};