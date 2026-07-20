// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Graph/Factory/DialogueGraphPinConnectionFactory.h"

#include "Graph/DialogueGraphSchema.h"
#include "Graph/Policy/DialogueGraphConnectionDrawingPolicy.h"


FConnectionDrawingPolicy* FDialogueGraphPinConnectionFactory::CreateConnectionPolicy(
	const UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor,
	const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const
{
	if (Schema && Schema->IsA<UDialogueGraphSchema>())
	{
		return new FDialogueGraphConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}

	return nullptr;
}
