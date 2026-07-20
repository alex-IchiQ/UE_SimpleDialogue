// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Slate/SDialogueGraphPin.h"

void SDialogueGraphPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SGraphPinExec::Construct(SGraphPinExec::FArguments(), InPin);
	
	PinColorModifier = FLinearColor::White;
	bUsePinColorForText = true;
}
