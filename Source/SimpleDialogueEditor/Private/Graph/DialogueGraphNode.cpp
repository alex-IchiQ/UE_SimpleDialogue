// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Graph/DialogueGraphNode.h"

#include "DialogueNode.h"
#include "Graph/DialogueGraphTypes.h"

#include "EdGraph/EdGraphPin.h"

void UDialogueGraphNode::ReconstructNode()
{
	Modify();

	const TArray<UEdGraphPin*> OldPins = Pins;
	Pins.Reset();

	AllocateDefaultPins();
	
	for (UEdGraphPin* OldPin : OldPins)
	{
		if (UEdGraphPin* NewPin = FindPin(OldPin->PinName, OldPin->Direction))
		{
			NewPin->MovePersistentDataFromOldPin(*OldPin);
		}
		
		OldPin->Modify();
		OldPin->BreakAllPinLinks();
		
		DestroyPin(OldPin);
	}
}

void UDialogueGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, DialogueGraph::TransitionPinCategory, TEXT("In"));

	// A Choice exposes one output pin per authored transition; everything else has a single output.
	if (NodeInstance && NodeInstance->IsA<UDialogueNode_Choice>())
	{
		for (int32 Index = 0; Index < NodeInstance->Outputs.Num(); ++Index)
		{
			UEdGraphPin* Pin = CreatePin(EGPD_Output, DialogueGraph::TransitionPinCategory, *FString::Printf(TEXT("Out_%d"), Index));

			// Show the branch label on the pin, falling back to "Option N" while it is unnamed.
			const FText& Label = NodeInstance->Outputs[Index].Label;
			Pin->PinFriendlyName = Label.IsEmpty() ? FText::Format(NSLOCTEXT("Dialogue", "OptionN", "Option {0}"), FText::AsNumber(Index + 1)) : Label;
		}
	}
	else
	{
		CreatePin(EGPD_Output, DialogueGraph::TransitionPinCategory, TEXT("Out"));
	}
}

void UDialogueGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	if (!FromPin)
	{
		return;
	}

	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->Direction == EGPD_Input)
		{
			if (GetSchema()->TryCreateConnection(Pin, FromPin))
			{
				FromPin->GetOwningNode()->NodeConnectionListChanged();
			}
			
			break;
		}
	}
}

FText UDialogueGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (!NodeInstance)
	{
		return NSLOCTEXT("Dialogue", "InvalidNodeTitle", "Invalid Node");
	}

	const FText Title = NodeInstance->GetNodeTitle();
	if (NodeInstance->Tag.IsNone())
	{
		return Title;
	}

	return FText::Format(NSLOCTEXT("Dialogue", "NodeTitleWithTag", "{0} \"{1}\""), Title, FText::FromName(NodeInstance->Tag));
}

FLinearColor UDialogueGraphNode::GetNodeTitleColor() const
{
	if (NodeInstance && NodeInstance->IsA<UDialogueNode_Choice>())
	{
		return FLinearColor(0.15f, 0.35f, 0.75f); // choice — blue
	}
	return FLinearColor(0.12f, 0.45f, 0.20f);     // line/default — green
}
