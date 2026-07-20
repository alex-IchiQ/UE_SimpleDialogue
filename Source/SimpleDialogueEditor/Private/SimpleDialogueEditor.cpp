// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#include "SimpleDialogueEditor.h"

#include "EdGraphUtilities.h"
#include "Graph/Factory/DialogueGraphNodeFactory.h"
#include "Graph/Factory/DialogueGraphPinFactory.h"
#include "Graph/Factory/DialogueGraphPinConnectionFactory.h"

#define LOCTEXT_NAMESPACE "FSimpleDialogueEditorModule"

void FSimpleDialogueEditorModule::StartupModule()
{
	// Register the three visual factories that give the dialogue graph its look:
	// custom node widgets, exec-style pins, and flat connection wires.
	NodeFactory = MakeShared<FDialogueGraphNodeFactory>();
	FEdGraphUtilities::RegisterVisualNodeFactory(NodeFactory);

	PinFactory = MakeShared<FDialogueGraphPinFactory>();
	FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);

	PinConnectionFactory = MakeShared<FDialogueGraphPinConnectionFactory>();
	FEdGraphUtilities::RegisterVisualPinConnectionFactory(PinConnectionFactory);
}

void FSimpleDialogueEditorModule::ShutdownModule()
{
	if (NodeFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(NodeFactory);
		NodeFactory.Reset();
	}

	if (PinFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
		PinFactory.Reset();
	}

	if (PinConnectionFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualPinConnectionFactory(PinConnectionFactory);
		PinConnectionFactory.Reset();
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSimpleDialogueEditorModule, SimpleDialogueEditor)
