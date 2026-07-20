// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "Modules/ModuleManager.h"

class FDialogueGraphNodeFactory;
class FDialogueGraphPinFactory;
class FDialogueGraphPinConnectionFactory;

class FSimpleDialogueEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Supplies the custom Slate widget for dialogue graph nodes. */
	TSharedPtr<FDialogueGraphNodeFactory> NodeFactory;

	/** Supplies the exec-style widget for dialogue pins. */
	TSharedPtr<FDialogueGraphPinFactory> PinFactory;

	/** Supplies the custom wire-drawing policy (flat white wires, no arrowheads). */
	TSharedPtr<FDialogueGraphPinConnectionFactory> PinConnectionFactory;
};
