// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Compile/DialogueCompiler.h"

#include "DialogueAsset.h"
#include "DialogueNode.h"

#include "Graph/DialogueGraph.h"
#include "Graph/DialogueGraphNode.h"

#include "Graph/Default/DialogueGraphNode_Root.h"
#include "Graph/Default/DialogueGraphNode_Jump.h"

#include "EdGraph/EdGraphPin.h"

namespace
{
	/** Find a runtime node by its designer Tag (first match wins). */
	UDialogueNode* FindNodeByTag(const UDialogueAsset* Asset, FName Tag)
	{
		if (Tag.IsNone())
		{
			return nullptr;
		}

		for (UDialogueNode* Node : Asset->Nodes)
		{
			if (Node && Node->Tag == Tag)
			{
				return Node;
			}
		}
		return nullptr;
	}

	/** Follow an output pin to the runtime node it should target — through a Jump if present. */
	UDialogueNode* ResolveTargetInstance(const UDialogueAsset* Asset, const UEdGraphPin* OutputPin)
	{
		if (!OutputPin || OutputPin->LinkedTo.Num() == 0)
		{
			return nullptr; // unconnected -> dead end (valid)
		}

		UEdGraphNode* Node = OutputPin->LinkedTo[0]->GetOwningNode();
		
		if (const UDialogueGraphNode_Jump* Jump = Cast<UDialogueGraphNode_Jump>(Node))
		{
			return FindNodeByTag(Asset, Jump->TargetTag);
		}
		
		if (const UDialogueGraphNode* GNode = Cast<UDialogueGraphNode>(Node))
		{
			return GNode->GetInstance();
		}

		return nullptr;
	}
}

void FDialogueCompiler::Compile(UDialogueAsset* Asset)
{
	if (!Asset || !Asset->Graph)
	{
		return;
	}

	UEdGraph* Graph = Asset->Graph;

	// 1. Rebuild the runtime node list from the graph (drops orphans).
	Asset->Nodes.Reset();
	Asset->EntryNode = nullptr;

	for (UEdGraphNode* Node : Graph->Nodes)
	{
		if (const UDialogueGraphNode* GNode = Cast<UDialogueGraphNode>(Node))
		{
			if (UDialogueNode* Instance = GNode->GetInstance())
			{
				Asset->Nodes.Add(Instance);
			}
		}
	}

	// 2. Bake connections into each node's Outputs (keep authored Label/Condition).
	for (UEdGraphNode* Node : Graph->Nodes)
	{
		const UDialogueGraphNode* GNode = Cast<UDialogueGraphNode>(Node);
		if (!GNode)
		{
			continue;
		}

		UDialogueNode* Instance = GNode->GetInstance();
		if (!Instance)
		{
			continue;
		}

		// Output pins in creation order == Outputs index order.
		TArray<UEdGraphPin*> OutputPins;
		for (UEdGraphPin* Pin : GNode->Pins)
		{
			if (Pin->Direction == EGPD_Output)
			{
				OutputPins.Add(Pin);
			}
		}

		// One transition per output pin (SetNum keeps existing entries, grows a Line 0->1).
		Instance->Outputs.SetNum(OutputPins.Num());

		for (int32 Index = 0; Index < OutputPins.Num(); ++Index)
		{
			Instance->Outputs[Index].TargetNode = ResolveTargetInstance(Asset, OutputPins[Index]);
		}
	}

	// 3. Resolve the entry node from the Root's output.
	for (UEdGraphNode* Node : Graph->Nodes)
	{
		const UDialogueGraphNode_Root* Root = Cast<UDialogueGraphNode_Root>(Node);
		if (!Root)
		{
			continue;
		}

		Asset->EntryNode = ResolveTargetInstance(Asset, Root->GetOutputPin());
		break;
	}
}