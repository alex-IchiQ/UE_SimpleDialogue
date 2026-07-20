// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Graph/Default/DialogueGraphNode_Jump.h"

#include "Graph/DialogueGraph.h"
#include "Graph/DialogueGraphTypes.h"
#include "DialogueAsset.h"
#include "DialogueNode.h"

#define LOCTEXT_NAMESPACE "DialogueGraphNodeJump"

void UDialogueGraphNode_Jump::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, DialogueGraph::TransitionPinCategory, TEXT("In"));
}

FText UDialogueGraphNode_Jump::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return TargetTag.IsNone() ? LOCTEXT("JumpNone", "Jump: <pick a tag>") : FText::Format(LOCTEXT("JumpTo", "Jump \u2192 {0}"), FText::FromName(TargetTag));
}

FLinearColor UDialogueGraphNode_Jump::GetNodeTitleColor() const
{
	return FLinearColor(0.45f, 0.20f, 0.50f);
}

TArray<FName> UDialogueGraphNode_Jump::GetTargetTagOptions() const
{
	TArray<FName> Options;
	if (const UDialogueGraph* DlgGraph = Cast<UDialogueGraph>(GetGraph()))
	{
		if (const UDialogueAsset* Asset = DlgGraph->GetDialogueAsset())
		{
			for (const UDialogueNode* Node : Asset->Nodes)
			{
				if (Node && !Node->Tag.IsNone())
				{
					Options.AddUnique(Node->Tag);
				}
			}
		}
	}
	return Options;
}

#undef LOCTEXT_NAMESPACE