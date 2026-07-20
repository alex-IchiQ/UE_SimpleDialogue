// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Graph/Policy/DialogueGraphConnectionDrawingPolicy.h"


FDialogueGraphConnectionDrawingPolicy::FDialogueGraphConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj)
	: FConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements)
{
	ArrowImage = nullptr;
	ArrowRadius = FVector2D::ZeroVector;
}

void FDialogueGraphConnectionDrawingPolicy::DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, FConnectionParams& Params)
{
	FConnectionDrawingPolicy::DetermineWiringStyle(OutputPin, InputPin, Params);

	if (!OutputPin || !InputPin)
	{
		return;
	}
	
	Params.WireColor = FLinearColor::White;
	Params.WireThickness = 2.5f;
	Params.bDrawBubbles = false;
}
