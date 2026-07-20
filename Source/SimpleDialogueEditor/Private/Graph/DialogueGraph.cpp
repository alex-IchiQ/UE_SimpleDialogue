// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Graph/DialogueGraph.h"

#include "DialogueAsset.h"

UDialogueAsset* UDialogueGraph::GetDialogueAsset() const
{
	return Cast<UDialogueAsset>(GetOuter());
}
