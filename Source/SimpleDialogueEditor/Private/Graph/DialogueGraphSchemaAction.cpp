// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Graph/DialogueGraphSchemaAction.h"

#include "DialogueAsset.h"
#include "DialogueNode.h"

#include "Graph/DialogueGraph.h"
#include "Graph/DialogueGraphNode.h"
#include "Graph/Default/DialogueGraphNode_Jump.h"

#define LOCTEXT_NAMESPACE "DialogueGraphSchemaAction"

FDialogueGraphSchemaAction_NewNode::FDialogueGraphSchemaAction_NewNode(TSubclassOf<UDialogueNode> InNodeClass, FText InCategory, FText InMenuDesc, FText InTooltip)
	: FEdGraphSchemaAction(MoveTemp(InCategory), MoveTemp(InMenuDesc), MoveTemp(InTooltip), 0), NodeClass(InNodeClass)
{
}

UEdGraphNode* FDialogueGraphSchemaAction_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode)
{
	const UDialogueGraph* Graph = CastChecked<UDialogueGraph>(ParentGraph);
	UDialogueAsset* Asset = Graph->GetDialogueAsset();

	const FScopedTransaction Transaction(LOCTEXT("AddDialogueNode", "Add Dialogue Node"));
	
	ParentGraph->Modify();
	Asset->Modify();
	
	if (FromPin)
	{
		FromPin->Modify();
	}

	UDialogueNode* NodeInstance = NewObject<UDialogueNode>(Asset, NodeClass, NAME_None, RF_Transactional);
	Asset->Nodes.Add(NodeInstance);

	if (UDialogueNode_Choice* Choice = Cast<UDialogueNode_Choice>(NodeInstance))
	{
		Choice->Outputs.SetNum(2);
	}
	
	FGraphNodeCreator<UDialogueGraphNode> Creator(*ParentGraph);
	UDialogueGraphNode* GraphNode = Creator.CreateNode(bSelectNewNode);
	GraphNode->SetInstance(NodeInstance);
	GraphNode->NodePosX = Location.X;
	GraphNode->NodePosY = Location.Y;
	Creator.Finalize();

	if (FromPin)
	{
		GraphNode->AutowireNewNode(FromPin);
	}

	return GraphNode;
}



FDialogueGraphSchemaAction_NewJump::FDialogueGraphSchemaAction_NewJump(FText Category, FText MenuDesc, FText Tooltip)
		: FEdGraphSchemaAction(MoveTemp(Category), MoveTemp(MenuDesc), MoveTemp(Tooltip), 0)
{
	
}

UEdGraphNode* FDialogueGraphSchemaAction_NewJump::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode)
{
	const FScopedTransaction Transaction(NSLOCTEXT("DialogueGraphSchema", "AddJump", "Add Jump Node"));
	ParentGraph->Modify();

	FGraphNodeCreator<UDialogueGraphNode_Jump> Creator(*ParentGraph);
	UDialogueGraphNode_Jump* Node = Creator.CreateNode(bSelectNewNode);
	Node->NodePosX = Location.X;
	Node->NodePosY = Location.Y;
	Creator.Finalize();

	if (FromPin)
	{
		Node->AutowireNewNode(FromPin);
	}
	
	return Node;
}

#undef LOCTEXT_NAMESPACE
