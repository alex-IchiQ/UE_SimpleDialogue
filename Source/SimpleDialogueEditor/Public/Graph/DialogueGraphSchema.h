// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "DialogueGraphSchema.generated.h"

UCLASS()
class SIMPLEDIALOGUEEDITOR_API UDialogueGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()
public:
	virtual void GetGraphContextActions(FGraphContextMenuBuilder&) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual void CreateDefaultNodesForGraph(UEdGraph&) const override;

	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
};
