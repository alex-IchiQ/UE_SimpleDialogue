// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Graph/Default/DialogueGraphNode_Root.h"

#include "Graph/DialogueGraphTypes.h"

#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "DialogueGraphNodeRoot"

void UDialogueGraphNode_Root::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, DialogueGraph::TransitionPinCategory, TEXT("Start"));
}

FText UDialogueGraphNode_Root::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("RootTitle", "Entry");
}

FLinearColor UDialogueGraphNode_Root::GetNodeTitleColor() const
{
	return FLinearColor(0.55f, 0.45f, 0.10f);
}

UEdGraphPin* UDialogueGraphNode_Root::GetOutputPin() const
{
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->Direction == EGPD_Output)
		{
			return Pin;
		}
	}
	return nullptr;
}

#undef LOCTEXT_NAMESPACE