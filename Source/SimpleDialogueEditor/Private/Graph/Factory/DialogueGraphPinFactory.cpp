// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Graph/Factory/DialogueGraphPinFactory.h"

#include "Graph/DialogueGraphSchema.h"
#include "Slate/SDialogueGraphPin.h"

#include "EdGraph/EdGraphPin.h"

TSharedPtr<SGraphPin> FDialogueGraphPinFactory::CreatePin(UEdGraphPin* InPin) const
{
	// Style every pin that lives in a dialogue graph — Entry, Line, Choice and Jump alike —
	// by keying off the graph's schema rather than the owning node's type.
	if (InPin && InPin->GetSchema() && InPin->GetSchema()->IsA<UDialogueGraphSchema>())
	{
		return SNew(SDialogueGraphPin, InPin);
	}

	return nullptr;
}
