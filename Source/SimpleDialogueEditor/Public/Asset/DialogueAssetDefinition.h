// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "AssetDefinitionDefault.h"
#include "DialogueAssetDefinition.generated.h"

UCLASS()
class SIMPLEDIALOGUEEDITOR_API UDialogueAssetDefinition : public UAssetDefinitionDefault
{
	GENERATED_BODY()

public:
	virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override;
	
	virtual FText GetAssetDisplayName() const override;
	virtual FLinearColor GetAssetColor() const override;
	
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};
