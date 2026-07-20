// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "Factories/Factory.h"
#include "DialogueAssetFactory.generated.h"

UCLASS()
class SIMPLEDIALOGUEEDITOR_API UDialogueAssetFactory : public UFactory
{
	GENERATED_BODY()
public:
	UDialogueAssetFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
