// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Graph/DialogueGraphSchema.h"
#include "Graph/DialogueGraphSchemaAction.h"
#include "Graph/Default/DialogueGraphNode_Root.h"

#include "DialogueNode.h"

#include "EdGraph/EdGraphPin.h"


#define LOCTEXT_NAMESPACE "DialogueGraphSchema"

void UDialogueGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const FText Category = LOCTEXT("DialogueNodeCategory", "Dialogue");

	ContextMenuBuilder.AddAction(MakeShared<FDialogueGraphSchemaAction_NewNode>(UDialogueNode_Line::StaticClass(), Category,
		LOCTEXT("AddLine", "Add Line Node"), LOCTEXT("AddLineTip", "A single line of dialogue (speaker + text).")));

	ContextMenuBuilder.AddAction(MakeShared<FDialogueGraphSchemaAction_NewNode>(UDialogueNode_Choice::StaticClass(), Category,
		LOCTEXT("AddChoice", "Add Choice Node"), LOCTEXT("AddChoiceTip", "A branching player choice.")));

	ContextMenuBuilder.AddAction(MakeShared<FDialogueGraphSchemaAction_NewJump>(Category,
		LOCTEXT("AddJump", "Add Jump Node"), LOCTEXT("AddJumpTip", "Redirect a branch to a tagged node without a wire.")));
}

const FPinConnectionResponse UDialogueGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (!A || !B)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinNull", "Invalid pin."));
	}
	
	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("SameNode", "A node cannot connect to itself."));
	}
	
	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("SameDirection", "Outputs must connect to inputs."));
	}
	
	const bool bAIsOutput = A->Direction == EGPD_Output;
	const UEdGraphPin* OutputPin = bAIsOutput ? A : B;
	
	if (OutputPin->LinkedTo.Num() > 0)
	{
		return FPinConnectionResponse(bAIsOutput ? CONNECT_RESPONSE_BREAK_OTHERS_A : CONNECT_RESPONSE_BREAK_OTHERS_B, LOCTEXT("ReplaceLink", "Replace existing output connection."));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, FText::GetEmpty());
}

void UDialogueGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<UDialogueGraphNode_Root> Creator(Graph);
	UDialogueGraphNode_Root* Root = Creator.CreateNode();
	Root->NodePosX = 0;
	Root->NodePosY = 0;
	Creator.Finalize();

	SetNodeMetaData(Root, FNodeMetadata::DefaultGraphNode);
}

FLinearColor UDialogueGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor(0.9f, 0.65f, 0.15f);
}

#undef LOCTEXT_NAMESPACE
