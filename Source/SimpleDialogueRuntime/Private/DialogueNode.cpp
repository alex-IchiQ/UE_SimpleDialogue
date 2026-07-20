// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "DialogueNode.h"

UDialogueNode::UDialogueNode()
{
	Guid = FGuid::NewGuid();
}

FText UDialogueNode::GetNodeTitle() const
{
	return NSLOCTEXT("Dialogue", "NodeTitle", "Dialogue Node");
}

FText UDialogueNode_Line::GetNodeTitle() const
{
	return SpeakerName.IsEmpty() ? NSLOCTEXT("Dialogue", "LineTitle", "Line") : SpeakerName;
}

FText UDialogueNode_Choice::GetNodeTitle() const
{
	return NSLOCTEXT("Dialogue", "ChoiceTitle", "Choice");
}
