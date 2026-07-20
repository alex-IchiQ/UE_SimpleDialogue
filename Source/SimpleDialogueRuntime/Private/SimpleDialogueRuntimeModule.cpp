// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FSimpleDialogueRuntimeModule"

class FSimpleDialogueRuntimeModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FSimpleDialogueRuntimeModule::StartupModule()
{
}

void FSimpleDialogueRuntimeModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSimpleDialogueRuntimeModule, SimpleDialogueRuntime)
