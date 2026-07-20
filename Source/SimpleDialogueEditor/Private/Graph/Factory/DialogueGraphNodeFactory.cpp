// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Graph/Factory/DialogueGraphNodeFactory.h"

#include "Graph/DialogueGraphNode.h"
#include "Slate/SDialogueGraphNode.h"

TSharedPtr<SGraphNode> FDialogueGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (UDialogueGraphNode* DialogueNode = Cast<UDialogueGraphNode>(Node))
	{
		return SNew(SDialogueGraphNode, DialogueNode);
	}
	
	return nullptr;
}
