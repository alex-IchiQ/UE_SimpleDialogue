// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Asset/DialogueAssetFactory.h"

#include "DialogueAsset.h"

UDialogueAssetFactory::UDialogueAssetFactory()
{
	SupportedClass = UDialogueAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UDialogueAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UDialogueAsset>(InParent, Class, Name, Flags);
}
